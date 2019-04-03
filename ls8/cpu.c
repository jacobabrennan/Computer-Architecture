#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "cpu.h"
#include "operations.h"

#define DATA_LEN 6

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
        fprintf(stderr, "Could not find file: %s", path_program);
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
        fprintf(stderr, "Derp: %d\n", op_identifier);
        fprintf(stderr, "ERROR Unknown ALU unstruction: %d\n", cpu->IR);
        exit(1);
    }
    instruction(cpu, regA, regB);
}

void timer_advance(struct cpu *cpu, struct timespec *time_current)
{
    time_t seconds_current = time_current->tv_sec;
    clock_gettime(CLOCK_MONOTONIC, time_current);
    if(time_current->tv_sec > seconds_current)
    {
        cpu->registers[REGISTER_INTERRUPT_STATUS] &= 0b00000001;
    }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
    //
    load_operations();
    //
    struct timespec time_current;
    clock_gettime(CLOCK_MONOTONIC, &time_current);
    //
    int running = 1; // True until we get a HLT instruction
    while(running)
    {
        //
        timer_advance(cpu, &time_current);
        if(cpu->registers[REGISTER_INTERRUPT_STATUS] & 0b00000001)
        {
            // Later in the main instruction loop, you'll check to see if bit 0 of the IS register is set, and if it is, you'll push the registers on the stack, look up the interrupt handler address in the interrupt vector table at address 0xF8, and set the PC to it. Execution continues in the interrupt handler.
            for(int index_register=0; index_register < REGISTER_INTERRUPT_STATUS; index_register++)
            {
                op_45_PUSH(cpu, index_register, 0);
            }
            cpu->PC = cpu->ram[0xf8];
            // Then when an IRET instruction is found, the registers and PC are popped off the stack and execution continues normally.

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
                fprintf(stderr, "ERROR Unknown Jump instruction: %d", cpu->IR);
                exit(1);
            }
            instruction(cpu, operand_1, operand_2);
        }
        else
        {
            operation instruction = operations_cpu[cpu->IR & INSTRUCTION_IDENTIFIER];
            if(NULL == instruction)
            {
                fprintf(stderr, "ERROR Unknown CPU instruction: %d", cpu->IR);
                exit(1);
            }
            instruction(cpu, operand_1, operand_2);
            // 6. Move the PC to the next instruction.
            cpu->PC += (1 + count_operand);
        }
    }
    //
    unload_operations();
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
    // Subsequently, the program can be loaded into RAM starting at address 0x00.
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
