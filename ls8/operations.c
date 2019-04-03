#include "operations.h"

operation *load_operations()
{
    operation *operations = calloc(0x100, sizeof(operation));
    operations[0x00] = op_00_NOP;
    operations[0x01] = op_01_HLT;
    operations[0x11] = op_11_RET;
    operations[0x13] = op_13_IRET;
    operations[0x45] = op_45_PUSH;
    operations[0x46] = op_46_POP;
    operations[0x47] = op_47_PRN;
    operations[0x50] = op_50_CALL;
    operations[0x52] = op_52_INT;
    operations[0x54] = op_54_JMP;
    operations[0x55] = op_55_JEQ;
    operations[0x56] = op_56_JNE;
    operations[0x57] = op_57_JGT;
    operations[0x58] = op_58_JLT;
    operations[0x59] = op_59_JLE;
    operations[0x5a] = op_5a_JGE;
    operations[0x65] = op_65_INC;
    operations[0x66] = op_66_DEC;
    operations[0x69] = op_69_NOT;
    operations[0x82] = op_82_LDI;
    operations[0x83] = op_83_LD;
    operations[0x84] = op_84_ST;
    operations[0xa0] = op_a0_ADD;
    operations[0xa1] = op_a1_SUB;
    operations[0xa2] = op_a2_MUL;
    operations[0xa3] = op_a3_DIV;
    operations[0xa4] = op_a4_MOD;
    operations[0xa7] = op_a7_CMP;
    operations[0xa8] = op_a8_AND;
    operations[0xaa] = op_aa_OR;
    operations[0xab] = op_ab_XOR;
    operations[0xac] = op_ac_SHL;
    operations[0xad] = op_ad_SHR;
    return operations;
}
