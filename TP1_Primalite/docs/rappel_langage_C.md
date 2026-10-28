# Rappel du langage C et des commandes (niveau intermédiaire)

## installation de gcc 
- Tu as installé MSYS2. C'est une boîte à outils qui contient gcc et un gestionnaire de paquets, pacman. Elle s'est installée dans C:\msys64.
- Tu as installé gcc avec pacman, dans le terminal MSYS2 UCRT64 :
`pacman -Syu` met à jour la base de paquets.
`pacman -S mingw-w64-ucrt-x86_64-gcc make` installe le compilateur et make.
- gcc se retrouve dans C:\msys64\ucrt64\bin.
- Le dossier C:\msys64\ucrt64\bin est dans ton PATH, c'est pourquoi la commande gcc marche partout. Le PATH est la liste des dossiers où Windows cherche les programmes quand tu tapes une commande. 

## 1. Structure d'un programme

```c
#include <stdio.h>          // préprocesseur : inclut les déclarations

#define PI 3.14159          // macro : remplacement textuel

int carre(int x) {          // fonction : type_retour nom(params)
    return x * x;
}

int main(void) {            // point d'entrée
    printf("%d\n", carre(4));
    return 0;               // code de sortie (0 = OK)
}
```

## 2. Types de base

| Type | Taille usuelle | Format printf/scanf |
|---|---|---|
| `char` | 1 octet | `%c` (caractère), `%d` (nombre) |
| `int` | 4 octets (±2,1·10⁹) | `%d` |
| `long` | 4 (Windows) ou 8 (Linux) | `%ld` |
| `long long` | 8 octets (±9,2·10¹⁸) | `%lld` |
| `unsigned int` | 4 octets, ≥ 0 | `%u` |
| `float` | 4 octets | `%f` |
| `double` | 8 octets | `%f` (printf), `%lf` (scanf) |
| chaîne | `char[]` terminé par `\0` | `%s` |

- `sizeof(type)` donne la taille en octets.
- `typedef long long ll;` crée un alias.
- Cast : `(double) n`, `(int) x` (conversion explicite ; réel → entier = troncature).
- Il n'y a pas de vrai type booléen : `0` = faux, tout le reste = vrai (`<stdbool.h>` ajoute `bool`).

## 3. Opérateurs

| Catégorie | Opérateurs |
|---|---|
| Arithmétique | `+ - * / %` (`/` entre entiers = division entière) |
| Comparaison | `== != < <= > >=` |
| Logique | `&&` (ET), `\|\|` (OU), `!` (NON) — évaluation paresseuse |
| Affectation | `= += -= *= /= %=`, `i++`, `i--` |
| Bits | `& \| ^ ~ << >>` |
| Adresse / pointeur | `&x`, `*p` |

Piège classique : `if (x = 0)` (affecte) au lieu de `if (x == 0)` (compare).

## 4. Structures de contrôle

```c
if (c) {...} else if (c2) {...} else {...}
while (c) {...}
do {...} while (c);          // au moins une fois
for (int i = 0; i < n; i++) {...}
switch (x) { case 1: ...; break; default: ...; }
break;      // sort de la boucle
continue;   // passe à l'itération suivante
```

## 5. Fonctions

- Passage **par valeur** : la fonction reçoit une copie. Pour modifier l'original, on passe un **pointeur** : `void f(int *p) { *p = 5; }` appelé par `f(&x);`.
- Une fonction doit être **déclarée avant** son utilisation (ou avoir un prototype : `int A5(ll n);`).
- `void` = pas de valeur de retour.

## 6. Pointeurs, tableaux, chaînes

```c
int x = 5;
int *p = &x;        // p contient l'adresse de x
*p = 7;             // modifie x
int t[3] = {1,2,3}; // tableau, indices 0..2, aucun contrôle de bornes !
char s[] = "abc";   // 'a','b','c','\0'
```
- Un tableau passé à une fonction devient un pointeur vers son 1er élément.
- Pointeur de fonction : `int (*f)(long long)` ; on peut le stocker dans un tableau, le passer en paramètre.
- `NULL` = pointeur « vide » (à tester après `fopen`, `malloc`).
- Mémoire dynamique : `p = malloc(n * sizeof(int));` … `free(p);` (`<stdlib.h>`).
- Fonctions de chaînes (`<string.h>`) : `strlen`, `strcpy`, `strcmp`, `strcat`.

## 7. Entrées / sorties

| Fonction | Rôle |
|---|---|
| `printf(fmt, ...)` | affiche sur `stdout` |
| `scanf(fmt, &x)` | lit au clavier |
| `fopen(nom, mode)` | ouvre un fichier : `"r"` lecture, `"w"` écriture (écrase), `"a"` ajout |
| `fscanf(f, fmt, &x)` | lit dans un fichier ; renvoie le nombre de valeurs lues |
| `fprintf(f, fmt, ...)` | écrit dans un fichier |
| `fgets(buf, taille, f)` | lit une ligne |
| `fclose(f)` | ferme |
| `fflush(stdout)` | force l'écriture du tampon |

`stdout` = sortie standard, `stderr` = erreurs (`fprintf(stderr, ...)`), `stdin` = entrée.

## 8. Le préprocesseur

- `#include <x.h>` (système) / `#include "x.h"` (ton fichier).
- `#define N 10`, `#define CARRE(x) ((x)*(x))` (macros).
- `#ifdef / #ifndef / #endif` : compilation conditionnelle.
- À la compilation : `-DNOM=valeur` définit une macro (ex. `-DLIMITE_LINEAIRE=5000000LL`).

## 9. Fichiers multiples (notion)

- `.h` = déclarations (prototypes), `.c` = définitions.
- Compiler plusieurs fichiers : `gcc main.c outils.c -o prog.exe`.

## 10. Chaîne de compilation

```
tp1.c --préprocesseur--> --compilateur--> tp1.o --linker (+ libs: printf, sqrt...)--> tp1.exe
```

## 11. Commandes de manipulation des fichiers C

### Compiler (gcc / MinGW sous Windows)
```
gcc --version                              vérifier que gcc est installé
gcc tp1.c -o tp1.exe                       compiler
gcc tp1.c -o tp1.exe -lm                   lier la lib maths (sqrt) – nécessaire sous Linux
gcc -Wall -Wextra tp1.c -o tp1.exe         activer les avertissements (toujours le faire)
gcc -O2 tp1.c -o tp1.exe                   optimiser (-O0 aucune, -O2 courant, -O3 agressif)
gcc -g tp1.c -o tp1.exe                    infos de débogage (pour gdb)
gcc -DLIMITE_LINEAIRE=5000000LL tp1.c -o tp1.exe    redéfinir une macro
gcc -c tp1.c                               compiler seulement en tp1.o (sans linker)
gcc -E tp1.c                               voir le résultat du préprocesseur
gcc -S tp1.c                               produire l'assembleur (tp1.s)
```
#### Commande simple ou commande avec options ?
La commande ordinaire suffit : `gcc tp1.c -o tp1.exe`. Les options sont facultatives :

| Partie | Rôle | Obligatoire ? |
|---|---|---|
| `gcc tp1.c` | compile le fichier | oui |
| `-o tp1.exe` | nom du fichier produit (sans lui : `a.exe`) | non, mais pratique |
| `-Wall` | affiche tous les avertissements | non, mais conseillé |
| `-O2` | optimise le code (programme plus rapide) | non |
| `-lm` | lie la bibliothèque maths (`sqrt`) | non sous Windows, oui sous Linux |

> **Pour le TP :** `-O2` change les temps mesurés. Toute l'équipe doit compiler avec **les mêmes options** (ex. `gcc -O2 tp1.c -o tp1.exe`), sinon les temps ne sont pas comparables ; à indiquer dans le rapport. Sans `-O2`, les temps sont plus lents mais la comparaison entre algorithmes reste valable.
> Attention : `^C` affiché dans un terminal = Ctrl + C (interruption), ce n'est pas une partie de la commande.

Le compilateur **n'est pas installé par défaut sous Windows** : installer MinGW-w64 / MSYS2 (`pacman -S mingw-w64-ucrt-x86_64-gcc`) et ajouter son dossier `bin` au `PATH`.

### Exécuter
```
.\tp1.exe Test-3.txt                         PowerShell (le .\ est obligatoire)
tp1.exe Test-3.txt                           cmd.exe
./tp1                                        Linux / macOS
```

### Redirections (c'est le shell qui les gère, pas le C)
```
tp1.exe Test-3.txt > resultats.csv           stdout → fichier (écrase)
tp1.exe Test-3.txt >> resultats.csv          stdout → fichier (ajoute)
tp1.exe Test-3.txt 2> erreurs.txt            stderr → fichier
tp1.exe < entree.txt                         fichier → stdin
echo %ERRORLEVEL%                            cmd : code de sortie du dernier programme
echo $LASTEXITCODE                           PowerShell : idem
```
Attention PowerShell 5 : `>` écrit en UTF-16 ; utiliser `| Out-File -Encoding ascii fichier.csv` ou `cmd /c "tp1.exe f.txt > r.csv"` pour un CSV propre.

### Chronométrer tout le programme (PowerShell)
```
Measure-Command { .\tp1.exe Test-3.txt > r.csv }
```

### Naviguer / gérer les fichiers (PowerShell | Linux)
| Action | PowerShell | Linux |
|---|---|---|
| dossier courant | `pwd` | `pwd` |
| aller dans | `cd data` | `cd data` |
| lister | `ls` / `dir` | `ls -l` |
| afficher un fichier | `Get-Content f.txt` | `cat f.txt` |
| 10 premières lignes | `Get-Content f.txt -TotalCount 10` | `head f.txt` |
| compter les lignes | `(Get-Content f.txt).Count` | `wc -l f.txt` |
| copier / déplacer / supprimer | `cp`, `mv`, `rm` | idem |
| créer un dossier | `mkdir out` | `mkdir out` |

### Automatiser les 5 tests (PowerShell)
```powershell
foreach ($f in 'Random100','Random1000','Test-1','Test-2','Test-3') {
    cmd /c ".\tp1.exe data\$f.txt > resultats_$f.csv"
}
```

### Automatiser avec un Makefile (si `make` est installé)
```make
tp1.exe: tp1.c
	gcc -O2 -Wall tp1.c -o tp1.exe -lm
clean:
	del tp1.exe
```
Utilisation : `make`, `make clean`.

## 12. Débogage et vérification

- `gcc -Wall -Wextra` : lire **tous** les warnings.
- `gdb tp1.exe` (compilé avec `-g`) : `break main`, `run Test-3.txt`, `next`, `print n`, `continue`.
- `printf` de débogage sur `stderr` pour ne pas polluer le CSV.
- Erreurs fréquentes :
  | Message | Cause |
  |---|---|
  | `undefined reference to 'A5'` | fonction utilisée mais non définie / fichier non lié |
  | `undefined reference to 'sqrt'` | oublier `-lm` |
  | `implicit declaration of function` | oubli de `#include` ou fonction déclarée après usage |
  | `Segmentation fault` | pointeur NULL / hors tableau / `scanf` sans `&` |
  | `'gcc' is not recognized` | gcc absent du `PATH` |
  | résultat étrange avec `int` | débordement : utiliser `long long` + `%lld` |

## 13. Bonnes pratiques

1. Toujours initialiser les variables.
2. Toujours tester le retour de `fopen`, `malloc`, `scanf`.
3. Un `free` pour chaque `malloc` ; un `fclose` pour chaque `fopen`.
4. Utiliser des types assez grands (`long long`) pour les gros nombres.
5. Pour mesurer des performances : compiler avec les **mêmes options** (`-O`), répéter les mesures, empêcher le compilateur de supprimer le code mesuré (`volatile`).

