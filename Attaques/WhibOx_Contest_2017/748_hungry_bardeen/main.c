#ifndef SYMBOL
#define SYMBOL value
#include <stdio.h>
#include <stdlib.h>
#include "748.c"


int main(int argc, char *argv[])
{
	unsigned char out[16];
	unsigned char in[16];

	// Vérification de l'entrée
	if(argc != 2) {
		printf("Erreur : entrée attendu : ./exe 0123456789ABCDEF0123456789ABCDEF\n");
		exit(0);
	}

	// On passe la chaine de carractère donnée dans le bon format
	// (1 char représentant 1 hexa codé sur 8 bits -> 1 char contenant 2 hexa sur 8 bits)
	char *pos = argv[1];
	for(int i = 0; i<16; i++) {
		sscanf(pos, "%2hhx", &in[i]);
		pos += 2;
	}

	AES_128_encrypt(out, in);

	//printf("OUTPUT \t: 0x");
	for(int i = 0; i< 16; i++) {
		printf("%02x", out[i]);
	}
	printf("\n");


	return 0;
}

#endif