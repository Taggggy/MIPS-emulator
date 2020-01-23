#include "memoire.h"
#include <stdlib.h>
#include <stdio.h>

void ecritureMemoire(uint8_t* memoire, uint32_t adresse, uint32_t valeur, int taille)
//Ecrit dans la mémoire à l'adresse indiquée la valeur donnée (peut écrire sur plusieurs octets selon la taille donnée)
{
	if(adresse >= TAILLE_MEMOIRE || adresse < 0) perror("Out of memory");
	int i;
	for(i = 0; i < taille/8; i++)
	{
				memoire[adresse + i] = valeur >> (8*((taille/8) -1 -i));
	}
}


uint8_t lectureMemoire(uint8_t* memoire, uint32_t adresse)
//Renvoie la valeur présente dans la mémoire à l'index correspondant à l'adresse indiquée, -1 si l'index n'existe pas
{
	if(adresse >= TAILLE_MEMOIRE || adresse < 0) perror("Out of memory");
	return memoire[adresse];
}

void afficherMemoire(uint8_t* memoire)
//Affiche toutes les valeurs présentes dans la mémoire
{
	int i;
	printf("MEMOIRE :\n==========================================================\n");
	for(i = 0; i < TAILLE_MEMOIRE; i+=4)
	{
		printf("@%03d:\t%02X\t@%03d:\t%02X\t@%03d:\t%02X\t@%03d:\t%02X\n", i,memoire[i], i+1, memoire[i+1], i+2, memoire[i+2], i+3, memoire[i+3]);
	}
	printf("==========================================================\n");
}