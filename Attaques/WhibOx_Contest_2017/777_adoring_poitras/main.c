#ifndef SYMBOL
#define SYMBOL value
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include "777.c"


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

	/* On lance un processus fils qui s'occupera d'exécuter l'AES.
	 Le père stopera le fils au bout d'une seconde.
	 Le chiffré est affiché même s'il est faux car deadpool attend un retour.
	 Comme daredevil n'utilise pas les sorties de l'AES, ce n'est pas dérangeant.  */
	pid_t pid_fils = fork();

	if(pid_fils) {
		sleep(1);
		kill(pid_fils, SIGKILL);
		for(int i = 0; i< 16; i++) {
			printf("%02x", out[i]);
		}
		printf("\n");
	}	
	else {
		AES_128_encrypt(out, in);
		for(int i = 0; i< 16; i++) {
			printf("%02x", out[i]);
		}
		printf("\n");
	}

	return 0;
}

#endif