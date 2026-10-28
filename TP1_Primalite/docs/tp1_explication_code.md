# TP1 – Primalité : le code `tp1.c` expliqué ligne par ligne

> Rôle de cette séance : **Testeur** (étape 3). Pour tester, il faut comprendre l'étape 1 (données) et l'étape 2 (code).
> ⚠️ Le fichier `tp1.c` du dossier **ne contient pas A5 et A6** (elles sont seulement dans `lyna.md`). Voir la section 9.

---

## 0. Vue d'ensemble (la chaîne complète)

```
Étape 1 (TP0)           Étape 2 (TP1 code)              Étape 3 (TOI)                 Étape 4
script Python  ──────►  tp1.c  (6 algos A1..A6)  ─────►  tp1.exe Test-3.txt > r.csv ──► rapport
génère data/*.txt       + fonction de mesure du temps    graphiques depuis le CSV
```

**Problème** : étant donné un entier `n`, est-il premier ? (divisible seulement par 1 et lui-même.)
**But du TP** : comparer 6 façons de le tester et voir comment leur **temps** grandit avec `n` (complexité).

| Algo | Teste les diviseurs `i` de… | Pas | Complexité (en nombre de tours) |
|---|---|---|---|
| A1 | 2 → n-1 | +1 | O(n) |
| A2 | 2 → n/2 | +1 | O(n) (2× moins que A1) |
| A3 | (pair éliminé) 3 → n-2 | +2 | O(n) (≈ 2× moins que A1) |
| A4 | (pair éliminé) 3 → n/2 | +2 | O(n) (≈ 4× moins que A1) |
| A5 | 2 → √n | +1 | O(√n) |
| A6 | (pair éliminé) 3 → √n | +2 | O(√n) (2× moins que A5) |

Pourquoi n/2 suffit : un diviseur de `n` plus grand que n/2 (autre que n) n'existe pas.
Pourquoi √n suffit : si `n = a × b`, un des deux facteurs est ≤ √n. Donc si aucun diviseur ≤ √n → premier.
Pourquoi éliminer les pairs : si `n` est pair (et ≠ 2) il n'est pas premier ; sinon aucun diviseur pair ne peut le diviser → on saute les `i` pairs (pas de 2).

---

## 1. Les en-têtes `#include`

```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
```

`#include` est une **directive du préprocesseur** : *avant* la compilation, un programme appelé préprocesseur remplace la ligne par le **contenu du fichier d'en-tête** (`.h`). Un `.h` contient seulement des **déclarations** (« la fonction `printf` existe et ressemble à ça »). Le code réel est dans la bibliothèque standard, liée à la fin (édition de liens / *linking*). Les `< >` signifient « cherche dans les dossiers du compilateur » (avec `" "` on chercherait d'abord dans ton dossier).

| En-tête | Ce qu'il apporte | Utilisé ici pour |
|---|---|---|
| `stdio.h` (**st**an**d**ard **i**nput/**o**utput) | entrées/sorties : `printf`, `fopen`, `fscanf`, `fclose`, `fflush`, type `FILE`, `stdout` | afficher le CSV, lire le fichier de nombres |
| `stdlib.h` | utilitaires généraux (`malloc`, `atoi`, `exit`, `rand`…) | **pas vraiment utilisé** ici (inclus par habitude) |
| `time.h` | `clock()`, type `clock_t`, constante `CLOCKS_PER_SEC` | mesurer le temps |
| `math.h` | `sqrt`, `pow`, `floor`… | `sqrt` dans A5/A6 |

> Sous gcc/Linux, `math.h` demande `-lm` à la compilation (lier la bibliothèque maths). Sous MinGW/Windows souvent inutile.

---

## 2. `typedef long long ll;`

```c
typedef long long ll;
```
- `typedef` = donner un **alias** (un surnom) à un type existant. Ici `ll` = `long long`.
- `long long` : entier sur **64 bits** (≈ ±9,2 × 10¹⁸). `int` fait en général 32 bits (max ≈ 2,1 × 10⁹) → **trop petit** pour nos nombres à 12 chiffres (ex. 550 998 984 401 > 2,1 × 10⁹). C'est pourquoi tout est en `ll`.
- Pour lire/écrire un `long long` avec `printf/scanf` on utilise le format **`%lld`**.

---

## 3. `#define LIMITE_LINEAIRE 1000000000LL`

```c
#define LIMITE_LINEAIRE 1000000000LL
```
- `#define NOM valeur` : **macro** du préprocesseur. Avant compilation, chaque `LIMITE_LINEAIRE` est remplacé *textuellement* par `1000000000LL`. Ce n'est **pas** une variable (pas de mémoire, pas de type, pas de `;`).
- Le suffixe `LL` force la constante à être un `long long` (sinon un trop gros littéral poserait problème).
- Rôle : au-delà de 10⁹, A1–A4 (en O(n)) feraient ~10⁹ tours **par appel** et on répète l'appel → des heures. On les **saute** (résultat `-1` = « non mesuré »).
- Modifiable à la compilation : `gcc -D LIMITE_LINEAIRE=5000000LL ...` (`-D` définit une macro en ligne de commande).

Commentaires : `/* ... */` (multi-ligne) et `// ...` (jusqu'à la fin de ligne).

---

## 4. Les algorithmes

Structure commune d'une **fonction C** :
```c
type_retour nom(type_param param) { ... return valeur; }
```
Ici : `int A1(ll n)` → prend un `long long`, renvoie un `int` : **1 = premier, 0 = non premier** (convention booléenne du C : 0 = faux, tout le reste = vrai ; il n'y a pas de type booléen "classique").

### A1 – brute force

```c
int A1(ll n) {
    int premier = 1;          // on SUPPOSE premier jusqu'à preuve du contraire (drapeau / flag)
    ll i = 2;                 // premier diviseur candidat
    while (i <= n - 1 && premier == 1) {   // tant qu'il reste des candidats ET qu'aucun diviseur trouvé
        if (n % i == 0) premier = 0;       // i divise n  → pas premier (on sort au prochain test)
        else i = i + 1;                    // sinon candidat suivant
    }
    return premier;
}
```
- `%` = **modulo** : reste de la division entière. `n % i == 0` ⇔ `i` divise `n`.
- `==` compare ; `=` affecte (confusion classique !).
- `&&` = ET logique. Évaluation **paresseuse** (court-circuit) : si la 1re condition est fausse, la 2e n'est pas évaluée.
- `while (cond) { ... }` : répète tant que `cond` est vraie.
- **Pourquoi le `else`** : quand on trouve un diviseur, on ne fait pas `i++`, mais `premier` passe à 0 donc la boucle s'arrête de toute façon.
- **Meilleur cas** : `n` pair → arrêt dès `i = 2` (1 tour). **Pire cas** : `n` premier → n-2 tours complets. **C'est pour ça que les données sont des nombres premiers** : on teste le pire cas.

#### Le commentaire sur `volatile` dans ton code
`volatile` dit au compilateur « cette variable peut changer sans que tu le voies, **ne l'optimise pas** : relis/écris-la en mémoire à chaque fois ». Le compilateur (avec `-O2`) peut sinon supprimer une boucle dont le résultat semble inutile, et le temps mesuré serait ~0. Dans le code actuel, `volatile` n'est **pas** appliqué à `i` (la ligne est en commentaire) ; il est appliqué à `res` dans `mesurer` (section 5), ce qui est l'endroit essentiel.

### A2
Identique, mais condition `i <= n / 2`. En C, `/` entre deux entiers = **division entière** (7/2 = 3), exactement le « div » du pseudo-code.

### A3 – pairs éliminés
```c
int A3(ll n) {
    int premier;  ll i;
    if (n != 2) {                    // 2 est le seul nombre pair premier
        if (n % 2 == 0) premier = 0; // pair ≠ 2 → non premier immédiatement
        else {
            premier = 1;
            i = 3;                   // on commence à 3
            while (i <= n - 2 && premier == 1) {
                if (n % i == 0) premier = 0;
                else i = i + 2;      // pas de 2 : 3,5,7,9,... (impairs seulement)
            }
        }
    } else premier = 1;              // n == 2
    return premier;
}
```
(Ici `premier` n'est pas initialisé à la déclaration mais est affecté dans **chaque** branche avant le `return` — c'est correct.)

### A4
Comme A3 avec `i <= n / 2`.

### A5 / A6 (dans `lyna.md`)
- `(ll) sqrt((double) n)` : `sqrt` travaille en `double` (réel 64 bits), donc **cast** `(double)` du `n` ; puis `(ll)` re-convertit le résultat en entier (troncature). Calculé **une seule fois** avant la boucle (sinon on recalculerait la racine à chaque tour = lent).
- Remarque : un `double` représente exactement les entiers jusqu'à 2⁵³ ≈ 9 × 10¹⁵ → OK pour 12 chiffres.
- Pour n=1 : A5 renvoie 1 (boucle vide) alors que 1 n'est pas premier — non gênant car les données ne contiennent que des premiers ≥ 2.

---

## 5. La mesure du temps : `mesurer`

```c
double mesurer(int (*f)(ll), ll n) {
    volatile int res = 0;
    long reps = 0;
    double el;
    clock_t deb = clock();
    do {
        res = f(n);
        reps++;
        el = (double)(clock() - deb) / CLOCKS_PER_SEC;
    } while (el < 0.05);
    (void) res;
    return el / reps;
}
```

- **`int (*f)(ll)`** = **pointeur de fonction** : `f` est une variable qui contient *l'adresse d'une fonction* prenant un `ll` et renvoyant un `int`. Lecture : « f est un pointeur (`*f`) vers une fonction `(ll)` qui renvoie `int` ». Les parenthèses autour de `*f` sont obligatoires (sinon ce serait une fonction qui renvoie un `int*`). Grâce à ça, **une seule** fonction `mesurer` sert pour A1…A6 : `mesurer(A3, n)` (le nom d'une fonction sans parenthèses = son adresse).
- **`clock()`** : renvoie le temps CPU consommé par le programme depuis son début, en « ticks » (type `clock_t`). `CLOCKS_PER_SEC` = nombre de ticks par seconde (1000 sous Windows). `(clock() - deb) / CLOCKS_PER_SEC` → secondes. Le `(double)` évite une division entière qui donnerait 0.
- **Pourquoi répéter (`do { } while`)** : la résolution de `clock()` est ~1 ms. Un appel de A6 sur 600 → quelques nanosecondes, donc impossible à mesurer en un coup. On **répète jusqu'à cumuler 0,05 s** puis on divise par le nombre de répétitions `reps` : **temps moyen d'un appel**.
- `do { ... } while (cond);` : comme `while`, mais le corps s'exécute **au moins une fois** (le `;` final est obligatoire).
- `reps++` : incrémente de 1 (équivaut à `reps = reps + 1`).
- **`volatile int res`** + `res = f(n)` : force le compilateur à vraiment *appeler* `f` à chaque tour (il ne peut pas dire « le résultat est inutile, je supprime l'appel »).
- `(void) res;` : utilise `res` pour faire taire l'avertissement « variable non utilisée ».

> ⚠️ Pour le rapport : cette mesure a une précision limitée (bruit de l'OS, autre processus, turbo CPU). Pour de très petites valeurs les temps affichés (ex. `0.000000012`) sont des moyennes sur des millions d'appels, donc plutôt fiables, mais pas parfaites.

### `nbChiffres`
```c
int nbChiffres(ll n) {
    int k = 0;
    while (n > 0) { k++; n /= 10; }   // on compte combien de fois on peut diviser par 10
    return k;
}
```
`n /= 10` ≡ `n = n / 10`. Le paramètre `n` est **passé par valeur** (une copie) → modifier `n` ici ne change pas le `n` de l'appelant. Sert à regrouper les résultats par nombre de chiffres dans les graphiques.

---

## 6. `main` : le programme principal

```c
int main(int argc, char *argv[]) {
```
- Point d'entrée : l'OS lance `main`. Le retour (`return 0`) est le **code de sortie** (0 = succès, ≠0 = erreur).
- `argc` = nombre d'arguments de la ligne de commande (**arg**ument **c**ount) ; `argv` = tableau de chaînes (**arg**ument **v**ector). Pour `tp1.exe Test-3.txt` : `argc = 2`, `argv[0] = "tp1.exe"`, `argv[1] = "Test-3.txt"`.
- `char *argv[]` : tableau de pointeurs vers `char` ; une chaîne en C = tableau de `char` terminé par `\0`.

```c
    if (argc < 2) {
        printf("Usage : tp1.exe fichier.txt\n");
        return 1;
    }
```
Si l'utilisateur n'a pas donné de fichier → message + sortie avec code 1 (erreur). `\n` = retour à la ligne. `printf` = affichage formaté sur la sortie standard.

```c
    FILE *f = fopen(argv[1], "r");
    if (f == NULL) { printf("Fichier %s introuvable\n", argv[1]); return 1; }
```
- `FILE *` : pointeur vers une structure de la bibliothèque qui représente un fichier ouvert (position de lecture, tampon…).
- `fopen(nom, "r")` : ouvre en lecture (`"r"` = read, `"w"` = write, `"a"` = append). Renvoie **`NULL`** si impossible (fichier absent) → on **toujours** vérifier.
- `%s` = format pour une chaîne.

```c
    int (*algo[6])(ll) = {A1, A2, A3, A4, A5, A6};
```
**Tableau de 6 pointeurs de fonctions**. `algo[0]` = A1 … `algo[5]` = A6. Permet de boucler sur les algos avec un indice `k`.

```c
    printf("N;chiffres;premier;T_A1;T_A2;T_A3;T_A4;T_A5;T_A6\n");
```
**En-tête du CSV** (séparateur `;`, ce que lit Excel en version française). Les lignes suivantes auront exactement ces 9 colonnes.

```c
    ll n;
    while (fscanf(f, "%lld", &n) == 1) {
```
- `fscanf(fichier, format, &variable)` : lit dans un fichier selon un format. `%lld` = lire un `long long`.
- **`&n`** = **adresse** de `n` : `fscanf` doit modifier `n`, donc on lui donne où l'écrire (sinon elle recevrait une copie).
- Renvoie le **nombre de valeurs lues avec succès** : 1 tant qu'il y a un nombre ; 0 ou `EOF` à la fin → la boucle s'arrête. Les espaces/retours à la ligne sont ignorés automatiquement.

```c
        printf("%lld;%d;%d", n, nbChiffres(n), A5(n));
```
Écrit le début de ligne : `N`, nombre de chiffres, et si `n` est premier (via A5, rapide). `%d` = `int`. Pas de `\n` : la ligne continue.

```c
        for (int k = 0; k < 6; k++) {
            double t = -1;                       /* -1 = non mesuré */
            if (k >= 4 || n <= LIMITE_LINEAIRE)  /* A5 et A6 toujours mesurés */
                t = mesurer(algo[k], n);
            printf(";%.9f", t);
        }
        printf("\n");
        fflush(stdout);
    }
```
- `for (init; condition; pas)` : boucle compteur. Ici `k` = 0..5.
- Condition : `k >= 4` (A5, A6 → indices 4 et 5) **OU** `||` n ≤ 10⁹ → on mesure. Sinon `t` reste `-1`.
- `%.9f` = réel avec 9 décimales (précision à la nanoseconde).
- `fflush(stdout)` : vide le tampon de sortie. `stdout` est normalement *bufferisé* (le texte est gardé en mémoire puis écrit par paquets). `fflush` force l'écriture **ligne par ligne** : si tu interromps le programme (très long), tu gardes déjà les lignes faites dans le CSV.

```c
    fclose(f);
    return 0;
}
```
Ferme le fichier (libère la ressource) puis termine avec succès.

---

## 7. Comment ça s'exécute en coulisse (compiler → exécuter)

1. **Préprocesseur** : traite `#include`, `#define` (copier-coller textuel).
2. **Compilateur** : traduit le C en code objet (langage machine, `.o`).
3. **Éditeur de liens (linker)** : relie ton code aux fonctions de la bibliothèque standard (`printf`, `sqrt`…) → fichier exécutable `tp1.exe`.
4. **Exécution** : l'OS charge `tp1.exe`, appelle `main(argc, argv)`.

Commande type : `gcc tp1.c -o tp1.exe -lm` (ajouter `-O2` pour optimiser : là `volatile` devient crucial).

**Redirection `>`** (du shell, pas du C) : `tp1.exe Test-3.txt > resultats_Test-3.csv` envoie tout ce que `printf` écrit (`stdout`) dans un fichier au lieu de l'écran. C'est pour ça que `main` n'affiche **que** le CSV (les messages d'erreur d'usage iraient aussi dans le fichier — détail à connaître).

---

## 8. Récapitulatif des concepts C à maîtriser

| Concept | Syntaxe | Idée |
|---|---|---|
| Directive préprocesseur | `#include`, `#define` | traitée avant compilation, texte pur |
| Alias de type | `typedef long long ll;` | surnom |
| Types | `int`, `long`, `long long`, `double`, `char` | tailles et plages différentes |
| Cast | `(double) n`, `(ll) x` | conversion explicite |
| Opérateurs | `% / * + -`, `== != < <= >= >`, `&& \|\| !` | `/` entier si deux entiers |
| Affectation composée | `i++`, `n /= 10`, `i += 2` | raccourcis |
| Conditions | `if / else` | 0 = faux, ≠0 = vrai |
| Boucles | `while`, `do…while`, `for` | |
| Fonctions | `type nom(params){…return…}` | passage **par valeur** |
| Pointeur / adresse | `*p`, `&x` | `&x` = où est `x` ; `*p` = ce qu'il y a à cette adresse |
| Pointeur de fonction | `int (*f)(ll)` | fonction passée en paramètre |
| Tableau | `int (*algo[6])(ll)`, `argv[]` | indices à partir de 0 |
| Chaînes | `char *`, `%s` | terminées par `\0` |
| Entrées/sorties | `printf`, `fscanf`, `fopen`, `fclose`, `fflush` | formats : `%d %lld %f %.9f %s` |
| Arguments de main | `argc`, `argv` | ligne de commande |
| Temps | `clock()`, `clock_t`, `CLOCKS_PER_SEC` | temps CPU |
| `volatile` | `volatile int res` | interdit l'optimisation de l'accès |
| Code de retour | `return 0/1` | 0 = OK |
| Redirection shell | `> fichier.csv` | stdout → fichier |

---

## 9. Points d'attention pour TOI (le testeur)

1. **`tp1.c` est incomplet** : `A5` et `A6` sont **utilisées** (lignes 112, 118) mais **non définies** dans le fichier (elles sont seulement dans `lyna.md`). Compilé tel quel, il échouera (« undefined reference to A5 »). Le `tp1.exe` existant a sans doute été compilé avec une version complète. → Demande à l'équipe la version finale du `.c`, ou colle A5/A6 avant `main`.
2. `stdlib.h` inutile ; `volatile` est en commentaire sur `i` (A1) — pas un bug.
3. Pour le CSV : `-1` = non mesuré (n > 10⁹ pour A1–A4). Dans Excel/Python, **filtrer ces -1** avant de tracer (et ne pas les prendre pour un vrai temps).
4. Commandes à lancer (pour chaque fichier `data/*.txt`) :
   ```
   tp1.exe Random100.txt  > resultats_Random100.csv
   tp1.exe Random1000.txt > resultats_Random1000.csv
   tp1.exe Test-1.txt     > resultats_Test-1.csv
   tp1.exe Test-2.txt     > resultats_Test-2.csv
   tp1.exe Test-3.txt     > resultats_Test-3.csv
   ```
5. Les données (voir `lyna.md`) : `Random100/1000` (premiers aléatoires), `Test-1` (100 premiers entre 10³ et 10⁶), `Test-2` (10 premiers par nombre de chiffres de 3 à 12... ), `Test-3` (10 à 3, 6, 9 et 12 chiffres). Elles ne contiennent que des **premiers** = pire cas des algorithmes.
6. Graphiques utiles : temps moyen par nombre de chiffres (échelle **log**), un courbe par algo ; rapport A1/A5 ; vérifier que la théorie tient (O(n) vs O(√n) : ×10 sur n → ×10 sur A1 mais ×≈3,16 sur A5).
7. Quand tu relances plusieurs fois, les temps varient légèrement : fermer les autres programmes, même machine pour tous les tests, et le préciser dans le rapport (CPU, OS, compilateur, options `-O`).


## 10. Comandes de test (par execution du fichier C sur les differentes donnees)

``` 
cmd /c ".\tp1.exe data\Test-1.txt > resultats\Test-1.csv"
cmd /c ".\tp1.exe data\Random100.txt > resultats\Random100.csv"
cmd /c ".\tp1.exe data\Random1000.txt > resultats\Random1000.csv"
cmd /c ".\tp1.exe data\Test-2.txt > resultats\Test-2.csv"
cmd /c ".\tp1.exe data\Test-3.txt > resultats\Test-3.csv"
```
