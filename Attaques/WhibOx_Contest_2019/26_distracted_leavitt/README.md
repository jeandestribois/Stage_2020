# Implémentation 26 distracted_leavitt du whibOx Contest de 2019

Ce dossier contient une attaque sur une l'implémentation numero 26 distracted_leavitt accessible à cette adresse : https://whibox-contest.github.io/2019/dashboard.

## Makefile

* La commande `make visualisation` permet de générer une trace mémoire, de la stocker sous forme de base de données sqlite et de l'afficher. Il est nécessaire d'avoir installé le plugin `TracerGrind` ainsi que les programme `sqlitetrace` et `tracegraph`.
* La commande `make enregistrement` permet de lancer l'enregistrement des traces (30 traces seront générées et converties dans un format que le programme `daredevil` accepte).
* La commande `make recherche` permet de lancer la recherche exhaustive sur les octets de clé manquant.

## CPA

Pour effectuer une CPA sur les traces générées, il est possible d'utliser les différents fichiers de configurations avec cette commande :

```bash
../../../Utiles/daredevil -c <un_fichier_de_configuration.conf>
```

par la suite on en déduit une partie de la clé étant `9D797E44B9CF850B21??8406??E3??4E` (les octets remplacés par `??` sont les octets que l'on a pas réussi à determiner).
Il faut donc lancer une recherche exhaustive sur ces octets.

## Exécution manuelle

Vous pouvez également modifier le fichier `enregistrement_traces.py` pour modifier la configuration de la génération des traces. Les fichiers de configuration édités automatiquement peuvent aussi être modifiés pour permettre une attaque plus ciblé (notament par rapport aux samples à considérer).