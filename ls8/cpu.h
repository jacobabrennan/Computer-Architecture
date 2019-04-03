#ifndef _CPU_H_
#define _CPU_H_

#define BYTE 0xff
#define INSTRUCTION_IDENTIFIER 0x0f
#define INSTRUCTION_OPERAND_COUNT 0b11000000
#define INSTRUCTION_ALU 0b00100000
#define INSTRUCTION_PC_MOVE 0b00010000
#define CMP_FLAG_EQUAL 0b00000001
#define CMP_FLAG_GREATER 0b00000010
#define CMP_FLAG_LESS 0b00000100
#define STACK_START 0xf4
#define REGISTER_STACK 7

// Holds all information about the CPU
struct cpu {
    unsigned char PC;
    unsigned char IR;
    unsigned char MAR;
    unsigned char MDR;
    unsigned char FL;
    unsigned char *registers;
    unsigned char *ram;
};

// Function declarations

extern void cpu_load(struct cpu *cpu, char *file_path);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address);
void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value);

#endif
