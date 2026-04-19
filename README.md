# Cartes Jumelles en C (Console)

Jeu de memoire en C11, version console.

Idee originale: M. Tahiri Hamza.
Realisation: Hamza Tahiri et Marwan Ouali.

## Concept

- Grille 4x4 de cartes cachees.
- Chaque carte a une copie exacte.
- Le joueur retourne 2 cartes.
- Si la paire est correcte: points gagnes.
- Si la paire est fausse: 1 vie perdue.
- Deux cartes pieges (!) sont melangees dans la grille.
- Si un piege est retourne: 2 vies perdues.
- Quand les vies arrivent a 0: partie terminee.
- Si toutes les paires non-pieges sont trouvees: victoire.

## Difficulte

- Facile: 6 vies
- Normal: 4 vies
- Difficile: 3 vies

## Structure

- src/main.c: boucle principale
- src/game.h, src/game.c: etat du jeu et logique des paires
- src/input.h, src/input.c: lecture des choix utilisateur
- src/render.h, src/render.c: affichage console du plateau
- src/persistence.h, src/persistence.c: meilleur score
- src/audio.h, src/audio.c: bips sonores simples (Windows)
- src/constants.h: constantes globales
- Makefile: compilation

## Compilation

### Windows (MSYS2 + MinGW64)

```bash
pacman -Sy --needed mingw-w64-x86_64-gcc mingw-w64-x86_64-make
mingw32-make
```

### Linux

```bash
sudo apt install build-essential
make
```

### macOS

```bash
brew install gcc
make
```

## Execution

### Windows

```bash
hamza_tahiri_et_marwan_ouali.exe
```

### Linux/macOS

```bash
./hamza_tahiri_et_marwan_ouali
```

## Controles

- Menu: 1/2/3 difficulte, 4 commencer, 0 quitter
- Partie: choisir les cartes avec leurs numeros (1 a 16)
- Partie: eviter les cartes pieges marquees !
- Fin de partie: 1 rejouer, 2 menu, 0 quitter

## Persistance

Le meilleur score est enregistre dans best_score.dat.
