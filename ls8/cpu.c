#include <stdlib.h>
#include <stdio.h>
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
    int address = 0;
    char line[0x100];
    while(fgets(line, 0x100, program))
    {
        char *index_error;
        unsigned char instruction = strtoul(line, &index_error, 2);
        if(line == index_error)
        {
            continue;
        }
        cpu->ram[address] = instruction;
        address++;
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

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
    int running = 1; // True until we get a HLT instruction
    load_operations();
    //
    while (running) {
        // TODO
        // 1. Get the value of the current instruction (in address PC).
        cpu->IR = cpu_ram_read(cpu, cpu->PC);
        // 2. Figure out how many operands this next instruction requires
        unsigned char count_operand = cpu->IR >> 6;
        // 3. Get the appropriate value(s) of the operands following this instruction
        unsigned char operand_1 = cpu_ram_read(cpu, cpu->PC+1);
        unsigned char operand_2 = cpu_ram_read(cpu, cpu->PC+2);
        // The above could be considered unsafe as it doesn't check if the
        // address is outside of addressable space. However, the last two bytes
        // of RAM as reserved, so any such attempt to read would already be an
        // error.
        // 4. switch() over it to decide on a course of action.
        // 5. Do whatever the instruction should do according to the spec.
        if(cpu->IR & INSTRUCTION_ALU)
        {
            alu(cpu, cpu->IR, operand_1, operand_2);
        }
        else
        {
            operation instruction = operations_cpu[cpu->IR];
            if(NULL == instruction)
            {
                fprintf(stderr, "ERROR Unknown unstruction: %d", cpu->IR);
                exit(1);
            }
            instruction(cpu, operand_1, operand_2);
        }
        // 6. Move the PC to the next instruction.
        cpu->PC += (1 + count_operand);
    }
    //
    unload_operations();
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
    cpu->PC = 0;
    cpu->IR = 0;
    cpu->MAR = 0;
    cpu->MDR = 0;
    cpu->FL = 0;
    cpu->registers = calloc(8, sizeof(char));
    cpu->ram = calloc(0x100, sizeof(char));
    cpu->registers[REGISTER_STACK] = STACK_START;
}

/**
 * Read from CPU RAM
 */
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
    return cpu->ram[address];
}

/**
 * Write to CPU RAM
 */
void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
    cpu->ram[address] = value;
}
