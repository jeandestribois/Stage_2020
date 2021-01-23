/*
 * Advanced Encryption Standard
 * @author Dani Huertas
 * @email huertas.dani@gmail.com
 *
 * Based on the document FIPS PUB 197
 */
#include <stdio.h>

#include "aes.h"

int main(int argc, char *argv[]) {

	uint8_t i;

	uint8_t key[] = {
		0x00, 0x01, 0x02, 0x03, 
		0x04, 0x05, 0x06, 0x07, 
		0x08, 0x09, 0x0a, 0x0b, 
		0x0c, 0x0d, 0x0e, 0x0f};

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
	uint8_t *w; // expanded key

	w = aes_init(sizeof(key));

	aes_key_expansion(key, w);

	aes_cipher(in, out, w);

	for (i = 0; i < 4; i++) {
		printf("%02x%02x%02x%02x", out[4*i+0], out[4*i+1], out[4*i+2], out[4*i+3]);
	}
	printf("\n");

	free(w);

	return 0;
}
