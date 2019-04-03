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
#define INTERRUPT_START 0xf8
#define REGISTER_INTERRUPT_MASK 5
#define REGISTER_INTERRUPT_STATUS 6
#define REGISTER_STACK_POINTER 7

// Holds all information about the CPU
struct cpu {
    unsigned char PC;
    unsigned char IR;
    unsigned char MAR;
    unsigned char MDR;
    unsigned char FL;
    unsigned char *registers;
    unsigned char *ram;
    //
    unsigned char running;
    unsigned char interruptable;
};

// Function declarations

extern void cpu_load(struct cpu *cpu, char *file_path);
extern void cpu_init(struct cpu *cpu);
extern void cpu_cleanup(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);
void cpu_ram_read(struct cpu *cpu);
void cpu_ram_write(struct cpu *cpu);

#endif
