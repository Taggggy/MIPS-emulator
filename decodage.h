#ifndef DECODAGE
#define DECODAGE

int TAILLE_PROGRAMME;
void conversionFichier(const char* fichierEntree, const char* fichierSortie);
char* conversionBinaire(const int aConvertir, const int taille);
char* operande(const char* instruction, int rangOperande);
char *decodeInstruction(char *bufEntree);


char *decode_add(const char* instruction);
char *decode_addi(const char* instruction);
char *decode_and(const char* instruction);
char *decode_beq(const char* instruction);
char *decode_bgtz(const char* instruction);
char *decode_blez(const char* instruction);
char *decode_bne(const char* instruction);
char *decode_div(const char* instruction);
char *decode_j(const char* instruction);
char *decode_jal(const char* instruction);
char *decode_jr(const char* instruction);
char *decode_lui(const char* instruction);
char *decode_lw(const char* instruction);
char *decode_mfhi(const char* instruction);
char *decode_mflo(const char* instruction);
char *decode_mult(const char* instruction);
char *decode_nop(const char* instruction);
char *decode_or(const char* instruction);
char *decode_rotr(const char* instruction);
char *decode_sll(const char* instruction);
char *decode_slt(const char* instruction);
char *decode_srl(const char* instruction);
char *decode_sub(const char* instruction);
char *decode_sw(const char* instruction);
char *decode_syscall(const char* instruction);
char *decode_xor(const char* instruction);

#endif
