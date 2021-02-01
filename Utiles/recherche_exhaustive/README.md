# Recherche exhaustive

Ce dossier contient mon implémentation permettant la recherche exhaustive sur certains octets
d'une clé de l'AES-128 bits.

## Compilation

Pour compiler le programme, il est nécessaire d'avoir `gcc` et d'entrer la commande suivante :

```bash
gcc recherche_exhaustive.c AES/aes.c AES/gmult.c -o recherche_exhaustive
```

## Exécution

Voici ce qu'il est nécessaire d'entrer pour exécuter le programme :

```bash
./recherche_exhaustive <cle> <clair> <chiffre>
```
Avec comme arguments :

* `cle` : La clé à chercher en hexadécimal (sans `0x`) avec chaque octets inconnus remplacés par `??`
* `clair` : Un message clair au choix en hexadécimal (sans `0x`)
* `chiffre` : Le chiffré du message clair choisi (sans `0x`)

Exemple : `./recherche_exhaustive 9D797E44B9CF850B21??8406??E3??4E 0123456789ABCDEF0123456789ABCDEF 6ee65987f3be09d3f451b8e8c850b563` 
