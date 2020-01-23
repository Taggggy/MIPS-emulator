#include "decodage.h"
#include "memoire.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


char *BIN_OPCODES[] = {"ADD", "ADDI", "AND", "BEQ", "BGTZ", "BLEZ", "BNE", "DIV", "J", "JAL", "JR", "LUI", "LW", "MFHI", "MFLO", "MULT", "NOP", "OR", "ROTR", "SLL", "SLT", "SRL", "SUB", "SW", "SYSCALL", "XOR"};
char *(*FCT_OPCODES[])(const char*) = {decode_add, decode_addi, decode_and, decode_beq, decode_bgtz, decode_blez, decode_bne, decode_div, decode_j, decode_jal, decode_jr, decode_lui, decode_lw, decode_mfhi, decode_mflo, decode_mult, decode_nop, decode_or, decode_rotr, decode_sll, decode_slt, decode_srl, decode_sub, decode_sw, decode_syscall, decode_xor};
const char *TXT_ALIAS[] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};

char *decodeInstruction(char *bufEntree)
//Décode l'instruction assembleur en instruction hexadécimale en lançant la fonction de décodage associée
{
	int i = 0;
	char *bufptrEntree = bufEntree;
	char *bufptrSortie;
	char opcode[8];
	char *instructionHexa = malloc(9*sizeof(char));
	//Suppression des espaces / tabulations
	while(isblank(bufptrEntree[i]))
		bufptrEntree++;
	//Suppression des commentaires
	while(bufptrEntree[i])
	{
		if(bufptrEntree[i] == '#')
			bufptrEntree[i] = '\0';
		else
			i++;
	}
	//Pour ne pas écrire les lignes vides
	if(strlen(bufptrEntree) <= 2)
		strcpy(instructionHexa,"");
	else
	{
		i = 0;
		// Obtention de l'OPCODE
		while (bufptrEntree[i] != ' ' &&  bufptrEntree[i] && bufptrEntree[i] != '\r' && bufptrEntree[i] != '\n')
		{
			opcode[i] = bufptrEntree[i];
			i++;
		}
		opcode[i] = '\0';

		i = 0;
		//Détermination de l'OPCODE correspondant
		while (strcmp(BIN_OPCODES[i], opcode))
		{
			i++;
		}
		//Fonction de décodage associée
		bufptrSortie = (*FCT_OPCODES[i])(bufptrEntree);
		sprintf(instructionHexa,"%08X",(unsigned int)strtol(bufptrSortie, NULL, 2));
		printf("%s\n", instructionHexa);
	}
	return instructionHexa;
}

void conversionFichier(const char* fichierEntree, const char* fichierSortie)
{
	//Ouvre le fichier d'entrée, traduit chacune des lignes en hexadécimale
	//et écrit en mémoire les instructions obtenues à partir de DEBUT PROGRAMME
	int lecture;
	size_t taille = 20;
	char* ligne = (char *)malloc(taille * sizeof(char));
	char hexa[9];
	FILE* fichierIn;
	FILE* fichierOut;
	//Ouverture des fichiers d'entrée et de sortie
	fichierIn = fopen(fichierEntree, "r");

	if (fichierSortie != NULL)
	{
		fichierOut = fopen(fichierSortie, "w+");
	}

	if(fichierIn == NULL) perror("Problème lors de l'ouverture du fichier d'entrée");
	if(fichierOut == NULL && fichierSortie != NULL) perror("Problème de l'écriture du fichier de sortie");
	while((lecture = getline(&ligne, &taille, fichierIn)) != -1) //Récupération d'une ligne jusqu'à la fin du fichier
	{
		strcpy(hexa,decodeInstruction(ligne));
        if(strlen(hexa)) //Si la ligne décodée n'est pas vide, on l'écrit
        {
					if (fichierSortie != NULL)
					{
        		fprintf(fichierOut, "%s\n", hexa);
					}

        	ecritureMemoire(memoire, TAILLE_PROGRAMME + DEBUT_PROGRAMME, strtol(hexa, NULL, 16), 32);
        	TAILLE_PROGRAMME += 4;
        }
    }
    fclose(fichierIn);

		if (fichierSortie != NULL)
		{
    	fclose(fichierOut);
		}
}

char* conversionBinaire(const int aConvertir, const int taille)
{
	//Convertit une valeur en une chaine de caractère binaire
	char* binaire = malloc((taille + 1)*sizeof(char));
	int i;
    for(i = taille-1; i >= 0; --i)
    {
        sprintf(binaire+(taille-1-i),"%c",(aConvertir & (1 << i)) ? '1' : '0');
    }
	binaire[(taille-1-i)] = '\0';
	return binaire;
}

char* operande(const char* instruction, int rangOperande)
//A partir d'une instruction, donne l'opérande au rang indiqué
{
	char* testFrontiere = malloc(20*sizeof(char));
	int i;
	testFrontiere = strchr(instruction, ' ') + 1;
	for(i = 0; i < rangOperande -1; i++)
		//Recherche d'un caractère séparateur d'opérande
		testFrontiere = strpbrk(testFrontiere, ",(") + 1;
	i = 0;
	while(testFrontiere[i] != ',' && testFrontiere[i] != '(' && testFrontiere[i] != ')' && i<strlen(testFrontiere))
		i++;

	char* operande = malloc(sizeof(char)*(i+1));
	strncpy(operande,testFrontiere,i);
	operande[i] = '\0';

	// Alias
	for (i = 0; i < 32; i++)
	{
		//Test de mnémoniques
		if (!strcmp(operande, TXT_ALIAS[i]))
		{
			strncpy(operande, "$ ", 3);
			operande[1] = i + '0';
		}
	}

	return operande;
}

char *decode_add(const char* instruction)
{
	char* instructionBinaire = malloc(33*sizeof(char));
	int tOperande;

	sprintf(instructionBinaire, "000000");
	tOperande = (int)strtol(operande(instruction,2)+1,NULL, 10);
	sprintf(instructionBinaire+6, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,3)+1,NULL, 10);
	sprintf(instructionBinaire+6+5, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,1)+1,NULL, 10);
	sprintf(instructionBinaire+6+5+5, conversionBinaire(tOperande,5));
	sprintf(instructionBinaire+6+5+5+5, "00000");
	sprintf(instructionBinaire+6+5+5+5+5, "100000");
	return instructionBinaire;
}
char *decode_addi(const char* instruction)
{
	char* instructionBinaire = malloc(33*sizeof(char));
	int tOperande;

	sprintf(instructionBinaire, "001000");
	tOperande = (int)strtol(operande(instruction,2)+1,NULL, 10);
	sprintf(instructionBinaire+6, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,1)+1,NULL, 10);
	sprintf(instructionBinaire+6+5, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,3),NULL, 10);
	sprintf(instructionBinaire+6+5+5, conversionBinaire(tOperande,16));
	return instructionBinaire;
}
char *decode_and(const char* instruction)
{
	char* instructionBinaire = malloc(33*sizeof(char));
	int tOperande;

	sprintf(instructionBinaire, "000000");
	tOperande = (int)strtol(operande(instruction,2)+1,NULL, 10);
	sprintf(instructionBinaire+6, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,3)+1,NULL, 10);
	sprintf(instructionBinaire+6+5, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,1)+1,NULL, 10);
	sprintf(instructionBinaire+6+5+5, conversionBinaire(tOperande,5));
	sprintf(instructionBinaire+6+5+5+5, "00000");
	sprintf(instructionBinaire+6+5+5+5+5, "100100");
	return instructionBinaire;
}
char *decode_beq(const char* instruction)
{
	char* instructionBinaire = malloc(33*sizeof(char));
	int tOperande;

	sprintf(instructionBinaire, "000100");
	tOperande = (int)strtol(operande(instruction,1)+1,NULL, 10);
	sprintf(instructionBinaire+6, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,2)+1,NULL, 10);
	sprintf(instructionBinaire+6+5, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,3),NULL, 10);
	sprintf(instructionBinaire+6+5+5, conversionBinaire(tOperande,16));
	return instructionBinaire;
}
char *decode_bgtz(const char* instruction)
{
	char* instructionBinaire = malloc(33*sizeof(char));
	int tOperande;

	sprintf(instructionBinaire, "000111");
	tOperande = (int)strtol(operande(instruction,1)+1,NULL, 10);
	sprintf(instructionBinaire+6, conversionBinaire(tOperande,5));
	sprintf(instructionBinaire+6+5, "00000");
	tOperande = (int)strtol(operande(instruction,2),NULL, 10);
	sprintf(instructionBinaire+6+5+5, conversionBinaire(tOperande,16));
	return instructionBinaire;
}
char *decode_blez(const char* instruction)
{
	char* instructionBinaire = malloc(33*sizeof(char));
	int tOperande;

	sprintf(instructionBinaire, "000110");
	tOperande = (int)strtol(operande(instruction,1)+1,NULL, 10);
	sprintf(instructionBinaire+6, conversionBinaire(tOperande,5));
	sprintf(instructionBinaire+6+5, "00000");
	tOperande = (int)strtol(operande(instruction,2),NULL, 10);
	sprintf(instructionBinaire+6+5+5, conversionBinaire(tOperande,16));
	return instructionBinaire;
}
char *decode_bne(const char* instruction)
{
	char* instructionBinaire = malloc(33*sizeof(char));
	int tOperande;

	sprintf(instructionBinaire, "000101");
	tOperande = (int)strtol(operande(instruction,1)+1,NULL, 10);
	sprintf(instructionBinaire+6, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,2)+1,NULL, 10);
	sprintf(instructionBinaire+6+5, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,3),NULL, 10);
	sprintf(instructionBinaire+6+5+5, conversionBinaire(tOperande,16));
	return instructionBinaire;
}
char *decode_div(const char* instruction)
{
	char* instructionBinaire = malloc(33*sizeof(char));
	int tOperande;

	sprintf(instructionBinaire, "000000");
	tOperande = (int)strtol(operande(instruction,1)+1,NULL, 10);
	sprintf(instructionBinaire+6, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,2)+1,NULL, 10);
	sprintf(instructionBinaire+6+5, conversionBinaire(tOperande,5));
	sprintf(instructionBinaire+6+5+5, "00000");
	sprintf(instructionBinaire+6+5+5+5, "00000");
	sprintf(instructionBinaire+6+5+5+5+5, "011010");
	return instructionBinaire;
}
char *decode_j(const char* instruction)
{
	char* instructionBinaire = malloc(33*sizeof(char));

	sprintf(instructionBinaire, "000010");
	sprintf(instructionBinaire+6, operande(instruction,1));
	return instructionBinaire;
}
char *decode_jal(const char* instruction)
{
	char* instructionBinaire = malloc(33*sizeof(char));

	sprintf(instructionBinaire, "000011");
	sprintf(instructionBinaire+6, operande(instruction,1));
	return instructionBinaire;
}
char *decode_jr(const char* instruction)
{
	char* instructionBinaire = malloc(33*sizeof(char));
	int tOperande;

	sprintf(instructionBinaire, "000000");
	tOperande = (int)strtol(operande(instruction,1)+1,NULL, 10);
	sprintf(instructionBinaire+6, conversionBinaire(tOperande,5));
	sprintf(instructionBinaire+6+5, "0000000000");
	sprintf(instructionBinaire+6+5+5+5, "00000"); //hint
	sprintf(instructionBinaire+6+5+5+5+5, "001000");
	return instructionBinaire;
}
char *decode_lui(const char* instruction)
{
	char* instructionBinaire = malloc(33*sizeof(char));
	int tOperande;

	sprintf(instructionBinaire, "001111");
	sprintf(instructionBinaire+6, "00000");
	tOperande = (int)strtol(operande(instruction,1)+1,NULL, 10);
	sprintf(instructionBinaire+6+5, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,2),NULL, 10);
	sprintf(instructionBinaire+6+5+5, conversionBinaire(tOperande,16));
	return instructionBinaire;
}
char *decode_lw(const char* instruction)
{
	char* instructionBinaire = malloc(33*sizeof(char));
	int tOperande;

	sprintf(instructionBinaire, "100011");
	tOperande = (int)strtol(operande(instruction,3)+1,NULL, 10);
	sprintf(instructionBinaire+6, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,1)+1,NULL, 10);
	sprintf(instructionBinaire+6+5, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,2),NULL, 10);
	sprintf(instructionBinaire+6+5+5, conversionBinaire(tOperande,16));
	return instructionBinaire;
}
char *decode_mfhi(const char* instruction)
{
	char* instructionBinaire = malloc(33*sizeof(char));
	int tOperande;

	sprintf(instructionBinaire, "000000");
	sprintf(instructionBinaire+6, "0000000000");
	tOperande = (int)strtol(operande(instruction,1)+1,NULL, 10);
	sprintf(instructionBinaire+6+5+5, conversionBinaire(tOperande,5));
	sprintf(instructionBinaire+6+5+5+5, "00000");
	sprintf(instructionBinaire+6+5+5+5+5, "010000");
	return instructionBinaire;
}
char *decode_mflo(const char* instruction)
{
	char* instructionBinaire = malloc(33*sizeof(char));
	int tOperande;

	sprintf(instructionBinaire, "000000");
	sprintf(instructionBinaire+6, "0000000000");
	tOperande = (int)strtol(operande(instruction,1)+1,NULL, 10);
	sprintf(instructionBinaire+6+5+5, conversionBinaire(tOperande,5));
	sprintf(instructionBinaire+6+5+5+5, "00000");
	sprintf(instructionBinaire+6+5+5+5+5, "010010");
	return instructionBinaire;
}
char *decode_mult(const char* instruction)
{
	char* instructionBinaire = malloc(33*sizeof(char));
	int tOperande;

	sprintf(instructionBinaire, "000000");
	tOperande = (int)strtol(operande(instruction,1)+1,NULL, 10);
	sprintf(instructionBinaire+6, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,2)+1,NULL, 10);
	sprintf(instructionBinaire+6+5, conversionBinaire(tOperande,5));
	sprintf(instructionBinaire+6+5+5, "0000000000");
	sprintf(instructionBinaire+6+5+5+5+5, "011000");
	return instructionBinaire;
}
char *decode_nop(const char* instruction)
{
	char* instructionBinaire = malloc(33*sizeof(char));
	sprintf(instructionBinaire, "00000000000000000000000000000000");
	return instructionBinaire;
}
char *decode_or(const char* instruction)
{
	char* instructionBinaire = malloc(33*sizeof(char));
	int tOperande;

	sprintf(instructionBinaire, "000000");
	tOperande = (int)strtol(operande(instruction,2)+1,NULL, 10);
	sprintf(instructionBinaire+6, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,3)+1,NULL, 10);
	sprintf(instructionBinaire+6+5, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,1)+1,NULL, 10);
	sprintf(instructionBinaire+6+5+5, conversionBinaire(tOperande,5));
	sprintf(instructionBinaire+6+5+5+5, "00000");
	sprintf(instructionBinaire+6+5+5+5+5, "100101");
	return instructionBinaire;
}
char *decode_rotr(const char* instruction)
{
	char* instructionBinaire = malloc(33*sizeof(char));
	int tOperande;

	sprintf(instructionBinaire, "000000");
	sprintf(instructionBinaire+6, "00001");
	tOperande = (int)strtol(operande(instruction,2)+1,NULL, 10);
	sprintf(instructionBinaire+6+5, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,1)+1,NULL, 10);
	sprintf(instructionBinaire+6+5+5, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,3),NULL, 10);
	sprintf(instructionBinaire+6+5+5+5, conversionBinaire(tOperande,5));
	sprintf(instructionBinaire+6+5+5+5+5, "000010");
	return instructionBinaire;
}
char *decode_sll(const char* instruction)
{
	char* instructionBinaire = malloc(33*sizeof(char));
	int tOperande;

	sprintf(instructionBinaire, "000000");
	sprintf(instructionBinaire+6, "00000");
	tOperande = (int)strtol(operande(instruction,2)+1,NULL, 10);
	sprintf(instructionBinaire+6+5, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,1)+1,NULL, 10);
	sprintf(instructionBinaire+6+5+5, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,3),NULL, 10);
	sprintf(instructionBinaire+6+5+5+5, conversionBinaire(tOperande,5));
	sprintf(instructionBinaire+6+5+5+5+5, "000000");
	return instructionBinaire;
}
char *decode_slt(const char* instruction)
{
	char* instructionBinaire = malloc(33*sizeof(char));
	int tOperande;

	sprintf(instructionBinaire, "000000");
	tOperande = (int)strtol(operande(instruction,2)+1,NULL, 10);
	sprintf(instructionBinaire+6, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,3)+1,NULL, 10);
	sprintf(instructionBinaire+6+5, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,1)+1,NULL, 10);
	sprintf(instructionBinaire+6+5+5, conversionBinaire(tOperande,5));
	sprintf(instructionBinaire+6+5+5+5, "00000");
	sprintf(instructionBinaire+6+5+5+5+5, "101010");
	return instructionBinaire;
}
char *decode_srl(const char* instruction)
{
	char* instructionBinaire = malloc(33*sizeof(char));
	int tOperande;

	sprintf(instructionBinaire, "000000");
	sprintf(instructionBinaire+6, "00000");
	tOperande = (int)strtol(operande(instruction,2)+1,NULL, 10);
	sprintf(instructionBinaire+6+5, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,1)+1,NULL, 10);
	sprintf(instructionBinaire+6+5+5, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,3),NULL, 10);
	sprintf(instructionBinaire+6+5+5+5, conversionBinaire(tOperande,5));
	sprintf(instructionBinaire+6+5+5+5+5, "000010");
	return instructionBinaire;
}
char *decode_sub(const char* instruction)
{
	char* instructionBinaire = malloc(33*sizeof(char));
	int tOperande;

	sprintf(instructionBinaire, "000000");
	tOperande = (int)strtol(operande(instruction,2)+1,NULL, 10);
	sprintf(instructionBinaire+6, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,3)+1,NULL, 10);
	sprintf(instructionBinaire+6+5, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,1)+1,NULL, 10);
	sprintf(instructionBinaire+6+5+5, conversionBinaire(tOperande,5));
	sprintf(instructionBinaire+6+5+5+5, "00000");
	sprintf(instructionBinaire+6+5+5+5+5, "100010");
	return instructionBinaire;
}
char *decode_sw(const char* instruction)
{
	char* instructionBinaire = malloc(33*sizeof(char));
	int tOperande;

	sprintf(instructionBinaire, "101011");
	tOperande = (int)strtol(operande(instruction,3)+1,NULL, 10);
	sprintf(instructionBinaire+6, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,1)+1,NULL, 10);
	sprintf(instructionBinaire+6+5, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,2),NULL, 10);
	sprintf(instructionBinaire+6+5+5, conversionBinaire(tOperande,16));
	return instructionBinaire;
}
char *decode_syscall(const char* instruction)
{
	char* instructionBinaire = malloc(33*sizeof(char));

	sprintf(instructionBinaire, "000000");
	sprintf(instructionBinaire+6, "00000000000000000000"); //code
	sprintf(instructionBinaire+6+5+5+5+5, "001100");
	return instructionBinaire;
}
char *decode_xor(const char* instruction)
{
	char* instructionBinaire = malloc(33*sizeof(char));
	int tOperande;

	sprintf(instructionBinaire, "000000");
	tOperande = (int)strtol(operande(instruction,2)+1,NULL, 10);
	sprintf(instructionBinaire+6, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,3)+1,NULL, 10);
	sprintf(instructionBinaire+6+5, conversionBinaire(tOperande,5));
	tOperande = (int)strtol(operande(instruction,1)+1,NULL, 10);
	sprintf(instructionBinaire+6+5+5, conversionBinaire(tOperande,5));
	sprintf(instructionBinaire+6+5+5+5, "00000");
	sprintf(instructionBinaire+6+5+5+5+5, "100110");
	return instructionBinaire;
}
