

//== Operations (Op Codes) ====================================================

//-- Dependencies ------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include "operations.h"

//-- Operation Array Memory Management ---------------
operation *operations_cpu;
operation *operations_cpu_jump;
operation *operations_alu;
void unload_operations()
{
    free(operations_cpu);
    free(operations_cpu_jump);
    free(operations_alu);
}
void load_operations()
{
    operations_cpu = calloc(0x10, sizeof(operation));
    // operations_cpu[0x0] = op_00_NOP;
    operations_cpu[0x1] = op_01_HLT;
    operations_cpu[0x2] = op_82_LDI;
    // operations_cpu[0x3] = op_83_LD;
    // operations_cpu[0x4] = op_84_ST;
    operations_cpu[0x5] = op_45_PUSH;
    operations_cpu[0x6] = op_46_POP;
    operations_cpu[0x7] = op_47_PRN;

    operations_cpu_jump = calloc(0x10, sizeof(operation));
    operations_cpu_jump[0x0] = op_50_CALL;
    // operations_cpu_jump[0x2] = op_52_INT;
    operations_cpu_jump[0x1] = op_11_RET;
    // operations_cpu_jump[0x3] = op_13_IRET;
    // operations_cpu_jump[0x4] = op_54_JMP;
    // operations_cpu_jump[0x5] = op_55_JEQ;
    // operations_cpu_jump[0x6] = op_56_JNE;
    // operations_cpu_jump[0x7] = op_57_JGT;
    // operations_cpu_jump[0x8] = op_58_JLT;
    // operations_cpu_jump[0x9] = op_59_JLE;
    // operations_cpu_jump[0xa] = op_5a_JGE;

    operations_alu = calloc(0x10, sizeof(operation));
    operations_alu[0x0] = op_a0_ADD;
    // operations_alu[0x1] = op_a1_SUB;
    operations_alu[0x2] = op_a2_MUL;
    // operations_alu[0x3] = op_a3_DIV;
    // operations_alu[0x4] = op_a4_MOD;
    // operations_alu[0x5] = op_65_INC;
    // operations_alu[0x6] = op_66_DEC;
    // operations_alu[0x7] = op_a7_CMP;
    // operations_alu[0x8] = op_a8_AND;
    // operations_alu[0x9] = op_69_NOT;
    // operations_alu[0xa] = op_aa_OR;
    // operations_alu[0xb] = op_ab_XOR;
    // operations_alu[0xc] = op_ac_SHL;
    // operations_alu[0xd] = op_ad_SHR;
}

//-- Operation Handlers ------------------------------
// operations[0x00] = op_00_NOP;
void op_01_HLT(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    (void)(cpu);
    (void)(operand_1);
    (void)(operand_2);
    exit(0);
}
void op_11_RET(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    (void)(operand_1);
    (void)(operand_2);
    cpu->PC = cpu->ram[cpu->registers[REGISTER_STACK]];
    cpu->registers[REGISTER_STACK]++;
}
// operations[0x13] = op_13_IRET;
void op_45_PUSH(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    (void)(operand_2);
    cpu->registers[REGISTER_STACK]--;
    cpu->ram[cpu->registers[REGISTER_STACK]] = cpu->registers[operand_1];
}
void op_46_POP(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    (void)(operand_2);
    cpu->registers[operand_1] = cpu->ram[cpu->registers[REGISTER_STACK]];
    cpu->registers[REGISTER_STACK]++;
}
void op_47_PRN(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    (void)(operand_2);
    fprintf(stdout, "%d\n", cpu->registers[operand_1]);
}
void op_50_CALL(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    (void)(operand_2);
    cpu->registers[REGISTER_STACK]--;
    cpu->ram[cpu->registers[REGISTER_STACK]] = cpu->PC+2;
    cpu->PC = cpu->registers[operand_1];
}
// operations[0x52] = op_52_INT;
// operations[0x54] = op_54_JMP;
// operations[0x55] = op_55_JEQ;
// operations[0x56] = op_56_JNE;
// operations[0x57] = op_57_JGT;
// operations[0x58] = op_58_JLT;
// operations[0x59] = op_59_JLE;
// operations[0x5a] = op_5a_JGE;
// operations[0x65] = op_65_INC;
// operations[0x66] = op_66_DEC;
// operations[0x69] = op_69_NOT;
void op_82_LDI(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    cpu->registers[operand_1] = operand_2;
}
// operations[0x83] = op_83_LD;
// operations[0x84] = op_84_ST;
void op_a0_ADD(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    unsigned char result = BYTE & (cpu->registers[operand_1] + cpu->registers[operand_2]);
    cpu->registers[operand_1] = result;   
}
// operations[0xa1] = op_a1_SUB;
void op_a2_MUL(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2)
{
    unsigned char result = BYTE & (cpu->registers[operand_1] * cpu->registers[operand_2]);
    cpu->registers[operand_1] = result;
}
// operations[0xa3] = op_a3_DIV;
// operations[0xa4] = op_a4_MOD;
// operations[0xa7] = op_a7_CMP;
// operations[0xa8] = op_a8_AND;
// operations[0xaa] = op_aa_OR;
// operations[0xab] = op_ab_XOR;
// operations[0xac] = op_ac_SHL;
// operations[0xad] = op_ad_SHR;
