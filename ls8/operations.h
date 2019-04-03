#ifndef _OPERATIONS_H_
#define _OPERATIONS_H_

typedef void (*operation)(struct cpu *cpu, unsigned char operand_1, unsigned char operand_2);

operation *load_operations();
operation op_00_NOP;
operation op_01_HLT;
operation op_11_RET;
operation op_13_IRET;
operation op_45_PUSH;
operation op_46_POP;
operation op_47_PRN;
operation op_50_CALL;
operation op_52_INT;
operation op_54_JMP;
operation op_55_JEQ;
operation op_56_JNE;
operation op_57_JGT;
operation op_58_JLT;
operation op_59_JLE;
operation op_5a_JGE;
operation op_65_INC;
operation op_66_DEC;
operation op_69_NOT;
operation op_82_LDI;
operation op_83_LD;
operation op_84_ST;
operation op_a0_ADD;
operation op_a1_SUB;
operation op_a2_MUL;
operation op_a3_DIV;
operation op_a4_MOD;
operation op_a7_CMP;
operation op_a8_AND;
operation op_aa_OR;
operation op_ab_XOR;
operation op_ac_SHL;
operation op_ad_SHR;

#endif
