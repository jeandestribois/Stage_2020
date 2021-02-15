# Implémentation 777 adoring_poitras du whibOx Contest de 2017

Ce dossier contient une tentative d'attaque sur une l'implémentation numero 777 adoring_poitras accessible à cette adresse : https://whibox-contest.github.io/2017/dashboard. Lorsqu'on tente d'enregistrer la trace d'exécution de cette implémentation, l'enregistrement ne s'arrête pas. Je n'ai pas réussi à comprendre quelle était la raison et donc pour pouvoir travailler dessus, j'ai rajouté un timer dans le main qui stoppe l'exécution au bout d'une seconde. Ainsi l'AES ne sera pas exécuté entièrement et le chiffré obtenu ne sera pas correct mais ce n'est pas génant car daredevil cible le premier tour de l'AES et n'utilise pas les chiffrés pour faire la CPA. Les filtres utilisés dans deadpool ont dû également être changés pour pouvoir lancer daredevil sur les données qui semblent être intéressantes. Je n'ai quand même pas réussi à trouver la clé. 

## Makefile

* La commande `make visualisation` permet de générer une trace mémoire, de la stocker sous forme de base de données sqlite et de l'afficher. Il est nécessaire d'avoir installé le plugin `TracerGrind` ainsi que les programmes `sqlitetrace` et `tracegraph`.
* La commande `make enregistrement` permet de lancer l'enregistrement des traces (100 traces seront générées et converties dans un format que le programme `daredevil` accepte).

## CPA

Pour effectuer une CPA sur les traces générées, il est possible d'utliser les différents fichiers de configuration avec cette commande :

```bash
../../../Utiles/daredevil -c <un_fichier_de_configuration.conf>
```

## Exécution manuelle

Vous pouvez également modifier le fichier `enregistrement_traces.py` pour modifier la configuration de la génération des traces. Les fichiers de configuration édités automatiquement peuvent aussi être modifiés pour permettre une attaque plus ciblé (notament par rapport aux samples à considérer).