#include "cpu.h"
#include "operations.h"

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  char data[DATA_LEN] = {
    // From print8.ls8
    0b10000010, // LDI R0,8
    0b00000000,
    0b00001000,
    0b01000111, // PRN R0
    0b00000000,
    0b00000001  // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++) {
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
    int running = 1; // True until we get a HLT instruction

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
        // 6. Move the PC to the next instruction.
    }
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
    cpu->ram = calloc(0xFF, sizeof(char));
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
void cpu_ram_read(struct cpu *cpu, unsigned char address, unsigned char value)
{
    cpu->ram[address] = value;
}
