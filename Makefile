all: decodage.o memoire.o registre.o instructions.o main.o 
	gcc -o emul-mips decodage.o memoire.o registre.o instructions.o main.o

main.o: main.c
	gcc -g -c main.c -Wall -o main.o

decodage.o: decodage.c decodage.h
	gcc -g -c decodage.c -Wall -o decodage.o

memoire.o: memoire.c memoire.h
	gcc -g -c memoire.c -Wall -o memoire.o

registre.o: registre.c registre.h
	gcc -g -c registre.c -Wall -o registre.o

instructions.o: instructions.c instructions.h
	gcc -g -c instructions.c -Wall -o instructions.o

clean: 
	rm *.o
	rm emul-mips
