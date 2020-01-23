#!/bin/bash

# Ce fichier a pour but de convertir tous les fichiers de tests sous forme hexadécimale
for file in `cd Tests && find * | grep txt`; do
	./emul-mips -tests Tests/$file Hexified/$file
done

exit 0
