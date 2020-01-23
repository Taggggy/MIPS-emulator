#include "decodage.h"
#include "registre.h"
#include "memoire.h"
#include "instructions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mode_pas_a_pas(char *inputFile)
{
	char entree = 0;
	TAILLE_PROGRAMME = 0;
	conversionFichier(inputFile, NULL);
	printf("Le programme fait %d octets\n", TAILLE_PROGRAMME);
	printf("Il se trouve en mémoire à partir de l'adresse %d\n", DEBUT_PROGRAMME);
	PC = DEBUT_PROGRAMME;
	HI = 0;
	LO = 0;
	while(PC != DEBUT_PROGRAMME + TAILLE_PROGRAMME + 4)
	{
		while(entree != 'n')
		{

			if(entree != '\n')
			{
				printf("Press\tn to go to the next instruction\n");
				printf("\tr to see the registers\n");
				printf("\tm to see the memory\n");
			}
			entree = getchar();
			switch(entree)
			{
				case('n'):system("clear");decoderInstruction(recupererInstruction(PC));PC += 4;break;
				case('r'):system("clear");afficherRegistres();;break;
				case('m'):system("clear");afficherMemoire(memoire);break;
			}
		}
		entree = 0;
	}
	system("clear");
	printf("Execution du programme terminée\n");
}

void mode_test(char *inputFile, char *outputFile)
{
	conversionFichier(inputFile, outputFile);
}

void mode_interactif()
{
	char entree[40];
	char op = 0;
	char continuer = 1;

	while (continuer)
	{
		printf("Entrez une instruction :\n");

		entree[0] = 0;
		while (entree[0] == '\0' || entree[0] == '\n')
			fgets(entree, 39, stdin);

		entree[strlen(entree)-1] = '\0';

		if (!strcmp(entree, "EXIT"))
		{
			continuer = 0;
		}
		else
		{
			decoderInstruction(strtol(decodeInstruction(entree), NULL, 16));
			op = 0;
			while(op != 'n')
			{
				if(op != '\n')
				{
					printf("Press\tn to go to the next instruction\n");
					printf("\tr to see the registers\n");
					printf("\tm to see the memory\n");
				}
				op = getchar();
				switch(op)
				{
					case('n'):break;
					case('r'):system("clear");afficherRegistres();;break;
					case('m'):system("clear");afficherMemoire(memoire);break;
				}
			}
		}
	}
	system("clear");
	printf("Execution du programme terminée\n");
}

int main(int argc, char *argv[])
{
	int i;
	char *inputFile;
	char *outputFile;

	if (argc == 1) // mode interactif
		mode_interactif();
	else
	{
		for (i = 1; i < argc; i++) // On recherche les arguments parmi les paramètres
		{
			if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) // aide
			{
				printf("Usage (interactif): ./emul-mips\n");
				printf("Usage (mode pas à pas): ./emul-mips -pas [InputFile]\n");
				printf("Usage (tests unitaires): ./emul-mips -tests [InputFile] [OutputFile]\n");
				break;
			}
			else if (!strcmp(argv[i], "-pas")) // mode pas à pas
			{
				if (argc != 3)
					printf("Usage (mode pas à pas): ./emul-mips -pas [InputFile]\n");
				else
				{
					inputFile = argv[(i == 2 ? 3 : 2)]; // Permet d'utiliser ./emul-mips -pas [InputFile] ou ./emul-mips [InputFile] -pas
					mode_pas_a_pas(inputFile);
				}
				break;
			}
			else if (!strcmp(argv[i], "-tests")) // mode tests unitaires
			{
				if (argc != 4)
					printf("Usage (tests unitaires): ./emul-mips -tests [InputFile] [OutputFile]\n");
				else
				{
					inputFile = argv[(i == 2 ? 3 : 2)];
					outputFile = argv[(i == 2 ? 4 : 3)];
					mode_test(inputFile, outputFile);
				}
				break;
			}
		}
	}
	return 0;
}
