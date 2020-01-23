#include <stdint.h>

int32_t recupererInstruction(int adresse);
void decoderInstruction(int32_t instruction);

void executer_add(int32_t instruction);
void executer_addi(int32_t instruction);
void executer_and(int32_t instruction);
void executer_beq(int32_t instruction);
void executer_bgtz(int32_t instruction);
void executer_blez(int32_t instruction);
void executer_bne(int32_t instruction);
void executer_div(int32_t instruction);
void executer_j(int32_t instruction);
void executer_jal(int32_t instruction);
void executer_jr(int32_t instruction);
void executer_lui(int32_t instruction);
void executer_lw(int32_t instruction);
void executer_mfhi(int32_t instruction);
void executer_mflo(int32_t instruction);
void executer_mult(int32_t instruction);
void executer_nop(int32_t instruction);
void executer_or(int32_t instruction);
void executer_rotr(int32_t instruction);
void executer_sll(int32_t instruction);
void executer_slt(int32_t instruction);
void executer_srl(int32_t instruction);
void executer_sub(int32_t instruction);
void executer_sw(int32_t instruction);
void executer_syscall(int32_t instruction);
void executer_xor(int32_t instruction);