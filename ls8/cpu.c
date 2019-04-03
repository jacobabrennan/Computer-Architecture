#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include "cpu.h"
#include "operations.h"

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *path_program)
{
    //
    FILE *program = fopen(path_program, "r");
    //
    if(NULL == program)
    {
        fprintf(stderr, "\rCould not find file: %s", path_program);
        exit(1);
    }
    //
    cpu->MAR = 0;
    char line[0x100];
    while(fgets(line, 0x100, program))
    {
        char *index_error;
        cpu->MDR = strtoul(line, &index_error, 2);
        if(line == index_error)
        {
            continue;
        }
        cpu_ram_write(cpu);
        cpu->MAR++;
    }
}

/**
 * ALU
 */
void alu(struct cpu *cpu, unsigned char op, unsigned char regA, unsigned char regB)
{
    unsigned char op_identifier = op & INSTRUCTION_IDENTIFIER;
    operation instruction = operations_alu[op_identifier];
    if(NULL == instruction)
    {
        fprintf(stderr, "\rDerp: %d\n", op_identifier);
        fprintf(stderr, "\rERROR Unknown ALU unstruction: %x\n", cpu->IR);
        cpu->running = 0;
    }
    instruction(cpu, regA, regB);
}

void timer_advance(struct cpu *cpu, struct timespec *time_current)
{
    time_t seconds_current = time_current->tv_sec;
    clock_gettime(CLOCK_MONOTONIC, time_current);
    if(time_current->tv_sec > seconds_current)
    {
        cpu->registers[REGISTER_INTERRUPT_STATUS] |= 0b00000001;
    }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
    struct termios termios_p;
    int success = tcgetattr(0, &termios_p);
    if(0 != success)
    {
        exit(1);
    }
    struct termios termios_sane = termios_p;
    cfmakeraw(&termios_p);
    termios_p.c_cc[VMIN] = 0;
    termios_p.c_cc[VTIME] = 0;
    success = tcsetattr(0, TCSANOW, &termios_p);
    if(0 != success)
    {
        exit(1);
    }
    //
    double long instruction_count = 0;
    if(cpu->running)
    {
        return;
    }
    //
    struct timespec time_current;
    clock_gettime(CLOCK_MONOTONIC, &time_current);
    //
    cpu->running = 1; // True until we get a HLT instruction
    while(cpu->running)
    {
        // Keyboard Interrupt
        char keyboard_input;
        int success_read = read(0, &keyboard_input, 1);
        if(success_read != -1)
        {
            cpu->MAR = 0xf4;
            cpu->MDR = keyboard_input;
            cpu_ram_write(cpu);
            cpu->registers[REGISTER_INTERRUPT_STATUS] |= 0b00000010;
        }
        //
        timer_advance(cpu, &time_current);
        // Prior to instruction fetch, the following steps occur:
        // The IM register is bitwise AND-ed with the IS register and the results stored as maskedInterrupts.
        unsigned char maskedInterrupts = cpu->registers[REGISTER_INTERRUPT_MASK] & cpu->registers[REGISTER_INTERRUPT_STATUS];
        // Each bit of maskedInterrupts is checked, starting from 0 and going up to the 7th bit, one for each interrupt.
        for(unsigned char mask_bit = 0; mask_bit < 8; mask_bit++)
        {
            unsigned char mask = 1 << mask_bit;
            // If a bit is found to be set, follow the next sequence of steps. Stop further checking of maskedInterrupts. Note: See break statement.
            if(maskedInterrupts & mask)
            {
                // Disable further interrupts.
                cpu->interruptable = 0;
                // Clear the bit in the IS register.
                cpu->registers[REGISTER_INTERRUPT_STATUS] &= ~mask;
                // The PC register is pushed on the stack.
                cpu->registers[REGISTER_STACK_POINTER]--;
                cpu->ram[cpu->registers[REGISTER_STACK_POINTER]] = cpu->PC;
                // The FL register is pushed on the stack.
                cpu->registers[REGISTER_STACK_POINTER]--;
                cpu->ram[cpu->registers[REGISTER_STACK_POINTER]] = cpu->FL;
                // Registers R0-R6 are pushed on the stack in that order.
                for(int index_register=0; index_register < REGISTER_INTERRUPT_STATUS; index_register++)
                {
                    op_45_PUSH(cpu, index_register, 0);
                }
                // The address (vector in interrupt terminology) of the appropriate handler is looked up from the interrupt vector table.
                cpu->MAR = INTERRUPT_START + mask_bit;
                cpu_ram_read(cpu);
                // Set the PC is set to the handler address.
                cpu->PC = cpu->MDR;
                // Stop further checking of maskedInterrupts. (see note above)
                break;
            }
        }
        // 1. Get the value of the current instruction (in address PC).
        cpu->MAR = cpu->PC;
        cpu_ram_read(cpu);
        cpu->IR = cpu->MDR;
        // 2. Figure out how many operands this next instruction requires
        unsigned char count_operand = cpu->IR >> 6;
        // 3. Get the appropriate value(s) of the operands following this instruction
        cpu->MAR = cpu->PC+1;
        cpu_ram_read(cpu);
        unsigned char operand_1 = cpu->MDR;
        cpu->MAR = cpu->PC+2;
        cpu_ram_read(cpu);
        unsigned char operand_2 = cpu->MDR;
        instruction_count++;
        if(5000000 == instruction_count)
        {
            printf("\rEncountered Infinit Loop");
            break;
        }
        //printf("\r%x\n", cpu->IR);
        //Sleep(10);
        // The above could be considered unsafe as it doesn't check if the
        // address is outside of addressable space. However, the last two bytes
        // of RAM as reserved, so any such attempt to read would already be an
        // error.
        // 4. switch() over it to decide on a course of action.
        // 5. Do whatever the instruction should do according to the spec.
        if(cpu->IR & INSTRUCTION_ALU)
        {
            alu(cpu, cpu->IR, operand_1, operand_2);
            cpu->PC += (1 + count_operand);
        }
        else if(cpu->IR & INSTRUCTION_PC_MOVE)
        {
            operation instruction = operations_cpu_jump[cpu->IR & INSTRUCTION_IDENTIFIER];
            if(NULL == instruction)
            {
                fprintf(stderr, "\rERROR Unknown Jump instruction: %x", cpu->IR);
                break;
            }
            instruction(cpu, operand_1, operand_2);
        }
        else
        {
            operation instruction = operations_cpu[cpu->IR & INSTRUCTION_IDENTIFIER];
            if(NULL == instruction)
            {
                fprintf(stderr, "\rERROR Unknown CPU instruction: %x", cpu->IR);
                break;
            }
            instruction(cpu, operand_1, operand_2);
            // 6. Move the PC to the next instruction.
            cpu->PC += (1 + count_operand);
        }
    }
    success = tcsetattr(0, TCSANOW, &termios_sane);
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
    // When the LS-8 is booted, the following steps occur:
    // R0-R6 are cleared to 0.
    cpu->registers = calloc(8, sizeof(char));
    // R7 is set to 0xF4.
    cpu->registers[REGISTER_STACK_POINTER] = STACK_START;
    // PC and FL registers are cleared to 0.
    cpu->PC = 0;
    cpu->FL = 0;
    // RAM is cleared to 0.
    cpu->ram = calloc(0x100, sizeof(char));
    //
    cpu->IR = 0;
    cpu->MAR = 0;
    cpu->MDR = 0;
    cpu->running = 0;
    cpu->interruptable = 1;
    load_operations();
    // Subsequently, the program can be loaded into RAM starting at address 0x00.
}

/**
 * Initialize a CPU struct
 */
void cpu_cleanup(struct cpu *cpu)
{
    (void)(cpu);
    unload_operations();
}

/**
 * Read from CPU RAM
 */
void cpu_ram_read(struct cpu *cpu)
{
    cpu->MDR = cpu->ram[cpu->MAR];
}

/**
 * Write to CPU RAM
 */
void cpu_ram_write(struct cpu *cpu)
{
    cpu->ram[cpu->MAR] = cpu->MDR;
}
