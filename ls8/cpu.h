#ifndef _CPU_H_
#define _CPU_H_

#define MASK_BYTE 0xFF
#define CMP_FLAG_EQUAL 0b00000001
#define CMP_FLAG_GREATER 0b00000010
#define CMP_FLAG_LESS 0b00000100

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

// ALU operations
enum alu_op {
	ALU_MUL
	// Add more here
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define HLT  0b00000001
#define PRN  0b01000111
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *file_path);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address);
void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value);

#endif
