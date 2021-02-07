// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License version 2 as published
// by the Free Software Foundation.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include "aes.h"
#include "aes_rp.h"
#include "aes_rp_prg.h"
#include "share.h"
#include "aes_share.h"
#include "aes_htable.h"
#include "common.h"
#include "prg.h"


int main(int argc, char *argv[])
{
  	unsigned char key[16]={0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
  	
  	unsigned char in[16]={0x32,0x43,0xf6,0xa8,0x88,0x5a,0x30,0x8d,0x31,0x31,0x98,0xa2,0xe0,0x37,0x07,0x34};

  	unsigned char out[16];

	// Cette variable correspond au nombre de masque à utiliser.
	// Pour un masquage d'ordre 1, il y a 2 masques et donc il faut mettre la varible n à 2.
	int n;

	// Vérification de l'entrée
	if(argc != 3) {
		printf("Erreur : entrée attendu : ./exe nombre_masque 0123456789ABCDEF0123456789ABCDEF\n");
		exit(0);
	}

	n = atoi(argv[1]);
	if(n < 1) {
		printf("Le masque doit être au moins égal à 1");
		exit(0);
	}

	char *pos = argv[2];
	for(int i = 0; i<16; i++) {
		sscanf(pos, "%2hhx", &in[i]);
		pos += 2;
	}

	/* On lance l'aes avec un masquage d'ordre n. les arguments sont :
		in 					: le clair
		out 				: la variable qui recevra le chiffré final
		key 				: la clé (ici définie un peu plus haut dans le main)
		n 					: le nombre de masque
		&subbyte_rp_share 	: le pointeur sur la fonction qui déroulera l'aes.
							Plusieurs implémentations de l'AES avec masquage sont proposés.
							Ici on prend la plus basique.
		1					: Le nombre de fois auquel on execute l'AES
	*/
	run_aes_share(in, out, key, n, &subbyte_rp_share, 1);


	for(int i = 0; i< 16; i++) {
		printf("%02x", out[i]);
	}
	printf("\n");

	return 0;
}