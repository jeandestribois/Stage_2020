# Implémentation basique de l'AES-128 bits

Ce dossier contient une attaque sur une implémentation que j'ai trouvé à cette adresse : https://github.com/dhuertas/AES.

## AES

Le dossier `AES` contient le code source de cette implémentation. Il a été nécessaire de changer le fichier `main.c` pour me permettre de simplifier ce que le programme attend en entrée et donne sortie.

## Makefile

* La commande `make visualisation` permet de générer une trace mémoire, de la stocker sous forme de base de données sqlite et de l'afficher. Il est nécessaire d'avoir installé le plugin `TracerGrind` ainsi que les programme `sqlitetrace` et `tracegraph`.
* La commande `make attaque` permet de lancer l'enregistrement des traces (30 traces seront générées et converties dans un format que le programme `daredevil` accepte) puis de lancer daredevil sur les données contenues dans la mémoire et l'attaque se faisant sur la position `AES_AFTER_SBOX`.

## Exécution manuelle

Vous pouvez également modifier le fichier `enregistrement_traces.py` pour modifier la configuration de la génération des traces. Les fichiers de configuration édités automatiquement peuvent aussi être modifiés pour permettre une attaque plus ciblé (notament par rapport aux samples à considérer).