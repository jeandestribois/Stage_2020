# Implémentation 748 hungry_bardeen du whibOx Contest de 2017

Ce dossier contient une tentative d'attaque sur une l'implémentation numero 748 hungry_bardeen disponible à cette adresse : https://whibox-contest.github.io/2017/dashboard. Sur cette implémentation, on ne retrouve pas le motif de l'AES graphiquement et donc je n'ai pas réussi à trouver où, en terme d'adresse, il serait efficace de cibler mon attaque. De plus lorsqu'on lance daredevil sur des traces complète de l'exécution, aucun octet de clé ne semble ressortir. J'ai essayé d'augmenter le nombre de traces (jusqu'à 2000) pour obtenir un résultat intéressant, mais ça ne semble rien changer. De ce fait, je n'ai pas réussi à trouver la clé pour cette implémentation. Ce problème est revenu pour plusieurs implémentations.

## Makefile

* La commande `make visualisation` permet de générer une trace mémoire, de la stocker sous forme de base de données sqlite et de l'afficher. Il est nécessaire d'avoir installé le plugin `TracerGrind` ainsi que les programmes `sqlitetrace` et `tracegraph`.
* La commande `make enregistrement` permet de lancer l'enregistrement des traces (200 traces seront générées et converties dans un format que le programme `daredevil` accepte).

## CPA

Pour effectuer une CPA sur les traces générées, il est possible d'utliser les différents fichiers de configuration avec cette commande :

```bash
../../../Utiles/daredevil -c <un_fichier_de_configuration.conf>
```

## Exécution manuelle

Vous pouvez également modifier le fichier `enregistrement_traces.py` pour modifier la configuration de la génération des traces. Les fichiers de configuration édités automatiquement peuvent aussi être modifiés pour permettre une attaque plus ciblé (notament par rapport aux samples à considérer).