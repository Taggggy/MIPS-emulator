#ifndef MEMOIRE
#define MEMOIRE

#include <stdint.h>
#define TAILLE_MEMOIRE 256
#define DEBUT_PROGRAMME 64

uint8_t memoire[TAILLE_MEMOIRE];

void ecritureMemoire(uint8_t* memoire, uint32_t adresse, uint32_t valeur, int taille);
uint8_t lectureMemoire(uint8_t* memoire, uint32_t adresse);
void afficherMemoire(uint8_t* memoire);

#endif