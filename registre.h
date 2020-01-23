#ifndef REGISTRE
#define REGISTRE

#include <stdint.h>

int32_t registreProc[32];
int32_t HI;
int32_t LO;
int32_t PC;

void ecritureRegistre(int32_t registre, int32_t valeur);
int32_t lectureRegistre(int32_t registre);
void afficherRegistres(void);

#endif