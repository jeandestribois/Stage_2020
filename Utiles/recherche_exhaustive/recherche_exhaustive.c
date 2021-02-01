#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "AES/aes.h"


/* Fonction permettant de vérifier si l'entrée est correcte */
void verification_entree(int argc, char *argv[]) {
	if(argc!=4) {
		printf("Erreur entrée : Pas assez d'arguments\n");
		exit(0);
	}

	/* Vérification de la taille des arguments */
	for(int i=1; i<4; i++) {
		if(strlen(argv[i]) != 32) {
			printf("Erreur entrée = Un argument ne fait pas la bonne taille");
			exit(0);
		}
	}

	/* Vérification de la clé */
	for(int j=0; j<32; j++) {
		if(!((argv[1][j]>='0' && argv[1][j]<='9')\
			|| argv[1][j] == '?'\
			|| (argv[1][j] >= 'a' && argv[1][j] <= 'f')\
			|| (argv[1][j] >= 'A' && argv[1][j] <= 'F'))) {
			printf("Erreur entrée : Un argument n'a pas le bon format.\n");
			exit(0);
		}
	}

	/* Vérification du message clair et chiffré */
	for(int i=2; i<4; i++) {
		for(int j=0; j<32; j++) {
			if(!((argv[i][j]>='0' && argv[i][j]<='9')\
				|| (argv[i][j] >= 'a' && argv[i][j] <= 'f')\
				|| (argv[i][j] >= 'A' && argv[i][j] <= 'F'))) {
				printf("Erreur entrée : Un argument n'a pas le bon format.\n");
				exit(0);
			}
		}
	}
}


/* Cette fonction permet de transformer les arguments dans le format attendu par l'aes */
void changement_format(char *argv[], uint8_t *cle, uint8_t *clair, uint8_t *chiffre, int *map) {
	// On transforme la clé dans le bon format
	// (1 char représentant 1 hexa codé sur 8 bits -> 1 char contenant 2 hexa sur 8 bits)
	char *pos = argv[1];
	for(int i = 0; i<16; i++) {
		if(pos[0] == '?' || pos[1] == '?') {
			map[i] = 1;
			cle[i] = 0;
		}
		else {
			map[i] = 0;
			sscanf(pos, "%2hhx", &cle[i]);
		}
		pos += 2;
	}

	// On transforme le message clair et le chiffré dans le bon format.
	pos = argv[2];
	for(int i=0; i<16; i++) {
		sscanf(pos, "%2hhx", &clair[i]);
		pos += 2;
	}
	pos = argv[3];
	for(int i=0; i<16; i++) {
		sscanf(pos, "%2hhx", &chiffre[i]);
		pos += 2;
	}
}


/* Permet d'afficher toutes les infos de la recherche */
void afficher_info(uint8_t *cle, uint8_t *clair, uint8_t *chiffre, int *map) {
	printf("\n********************************************************\n");
	printf("\nClé : \t\t0x");
	for (int i = 0; i < 16; i++) {
		if(map[i]) printf("??");
		else printf("%02x", cle[i]);
	}
	printf("\nMessage : \t0x");
	for (int i = 0; i < 16; i++) {
		printf("%02x", clair[i]);
	}
	printf("\nChiffré : \t0x");
	for (int i = 0; i < 16; i++) {
		printf("%02x", chiffre[i]);
	}
	printf("\n");
}

/* Effectue la recherche exhaustive des octets manquant */
uint8_t *recherche_cle(uint8_t *cle, uint8_t *clair, uint8_t *chiffre, int *map) {
	// On évalue le nombre d'octet à trouver.
	int nbre_octet_manquant=0;
	for(int i=0; i<16; i++) {
		nbre_octet_manquant += map[i];
	}

	// Variables utiles pour le calcul du chiffré
	uint8_t *cle_derivee = aes_init(sizeof(cle));
	uint8_t chiffre_potentiel[16];

	// Variable utile pour la modification de la clé
	int bit_change = 0;

	//Variable utile pour la comparaison des deux chiffrés
	int mauvais_chiffre = 0;

	printf("\nPourcentage de clés testées :\n");
	// Boucle principale de recherche exhaustive
	for(int i=0; i < (1<<(nbre_octet_manquant*8)); i++) {
		// On chiffre
		aes_key_expansion(cle, cle_derivee);
		aes_cipher(clair, chiffre_potentiel, cle_derivee);

		// On compare les chiffrés obtenu
		for(int i=0; i<16 && !mauvais_chiffre; i++) {
			if(chiffre_potentiel[i] != chiffre[i])
				mauvais_chiffre = 1;
		}
		if(mauvais_chiffre)
			mauvais_chiffre = 0;
		else
			return cle;

		// On applique la modification sur la clé
		for(int i=0; i<16 && !bit_change; i++) {
			if(map[i]) {
				if (cle[i] == 0xff) cle[i] = 0;
				else {
					cle[i] += 1;
					bit_change = 1;
				}
			}
		}
		bit_change = 0;

		// On affiche l'avancement
		printf("\r");
		printf("%d %%", i*100/((1<<(nbre_octet_manquant*8))+1));
	}

	return NULL;
}


int main(int argc, char *argv[])
{

	verification_entree(argc, argv);

	uint8_t cle[16];
	uint8_t clair[16];
	uint8_t chiffre[16];

	int map[16];		// Permet de connaitre les octets de la clé ciblés par la recherche.

	changement_format(argv, cle, clair, chiffre, map);

	afficher_info(cle, clair, chiffre, map);

	aes_init(16);

	uint8_t *cle_trouve = recherche_cle(cle, clair, chiffre, map);

	if(cle_trouve == NULL)
		printf("\nAucune clé trouvée...\n");
	else {
		printf("\nClé trouvée : \t\t0x");
		for(int i=0; i<16; i++) {
			printf("%02x", cle_trouve[i]);
		}
		printf("\n");
	}


	return 0;
}