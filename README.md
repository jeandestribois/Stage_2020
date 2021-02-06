# Stage_2020

Ce dépôt regroupe le travail que j'ai effectué durant mon stage dans l'équipe Crypto du laboratoire LMV de l'UVSQ. Il contient un récapitulatif apportant des détails sur les outils que j'ai utilisés, diverses implémentations en boîte blanche de l'AES-128 bits et des attaques (réussi ou non) sur celles-ci.

## Attaques

Ce dossier contient les attaques essayées sur les différentes implémentations de l'AES-128 bits (chacunes dans un dossier).

## Recapitulatif

Ce dossier contient un récapitulatif de mon stage et des détails que je souhaite apporter sur les outils et les attaques. Il contient également le code source en latex du récapitulatif (pour d'éventuelles modifications).

## Utiles

Ce dossier contient quelques fichiers/programmes utiles pour les attaques et pour l'installation :

* Le script `deadpool_dca.py` pour l'automatisation de la génération et du formatage des traces mémoire.
* Le programme `daredevil` pour la CPA sur les traces mémoires.
* Le dossier `recherche_exaustive` contenant mon implémentation de la recherche exhaustive sur certain octets de clé de l'AES-128 bits.
* Le fichier `valgrind-3.16.1.diff` permettant de faire un patch sur les fichiers de configuration de valgrind 3.16.1 lors de l'ajout du plugin TracerGrind.
