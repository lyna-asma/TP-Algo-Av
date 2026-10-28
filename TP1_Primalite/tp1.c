/* =====================================================================
 * TP1 - Primalite : comparaison de 6 algorithmes (A1..A6)
 * Usage : tp1.exe fichier.txt > resultats.csv
 * Le programme lit des entiers dans fichier.txt, et affiche sur la sortie
 * standard un CSV : N;chiffres;premier;T_A1;...;T_A6 (temps moyens en s).
 * ===================================================================== */

// #include = directive du PREPROCESSEUR : avant la compilation, la ligne est remplacee
// par le contenu du fichier d'en-tete (.h), qui contient les DECLARATIONS des fonctions.
// Le code reel de ces fonctions est ajoute plus tard par l'editeur de liens (linker).
// < > = on cherche l'en-tete dans les dossiers du compilateur (avec " " on cherche d'abord ici).
#include <stdio.h>   // standard input/output : printf, fopen, fscanf, fclose, fflush, type FILE, stdout
#include <stdlib.h>  // utilitaires generaux (malloc, exit, atoi...) : pas utilise ici, inclus par habitude
#include <time.h>    // clock(), type clock_t, constante CLOCKS_PER_SEC : pour mesurer le temps
#include <math.h>    // sqrt() : utilise dans A5 et A6 (avec gcc sous Linux, compiler avec -lm)

// typedef = donne un ALIAS (surnom) a un type existant : "ll" signifie desormais "long long".
// long long = entier sur 64 bits (jusqu'a ~9,2e18). int (32 bits, max ~2,1e9) est trop petit
// pour nos nombres a 12 chiffres. Pour printf/scanf, le format d'un long long est %lld.
typedef long long ll;

/* Au-dela de cette valeur, les algorithmes en O(n) sont trop lents : on les saute */
// define est une constante de compilation, on peut la modifier avec -D LIMITE_LINEAIRE=... lors de la compilation
// ll est un type entier long long, qui peut contenir des valeurs plus grandes que int ou long
// cette constante est specialement utile pour les algorithmes A1, A2, A3 et A4, qui sont en O(n) et donc trop lents pour de grandes valeurs de n
// #define NOM valeur : MACRO du preprocesseur = remplacement purement textuel de NOM par valeur
// avant la compilation. Ce n'est pas une variable (pas de type, pas de memoire, pas de ";").
// Le suffixe LL force la constante a etre un long long.
#define LIMITE_LINEAIRE 1000000000LL

/* ---------- Algorithme A1 : i de 2 a n-1 ---------- */
// Convention : chaque algo renvoie 1 si n est premier, 0 sinon (en C : 0 = faux, autre = vrai).
// Le parametre n est passe PAR VALEUR : la fonction travaille sur une copie.
int A1(ll n) {
    // "drapeau" (flag) : on SUPPOSE n premier jusqu'a ce qu'on trouve un diviseur
    int premier = 1;
    // l'intruction volatile empeche le compilateur d'optimiser la variable i, ce qui est important pour mesurer correctement le temps d'execution
    // ll i = 2;  // on peut aussi utiliser "volatile ll i = 2;" pour la meme raison
    // la volatilite est simplement une indication pour le compilateur que la variable peut changer de maniere inattendue, donc il ne doit pas optimiser son utilisation
    // par optimisation je veux dire que le compilateur pourrait, par exemple, remplacer la variable i par une valeur constante si il pense que i ne change pas, ce qui fausserait la mesure du temps d'execution
    ll i = 2;  // i = diviseur candidat courant, on commence a 2
    // && = ET logique. On continue tant qu'il reste des candidats ET qu'aucun diviseur n'a ete trouve
    while (i <= n - 1 && premier == 1) {
        // % = modulo (reste de la division entiere) : n % i == 0 <=> i divise n
        // (attention : == compare, = affecte)
        if (n % i == 0) premier = 0;  // diviseur trouve : n n'est pas premier, la boucle va s'arreter
        else i = i + 1;               // sinon on passe au candidat suivant (pas de 1)
    }
    return premier;  // renvoie le resultat a l'appelant
}

/* ---------- Algorithme A2 : i de 2 a n div 2 ---------- */
// Meme principe que A1, mais on s'arrete a n/2 : aucun diviseur de n (autre que n) n'est > n/2.
int A2(ll n) {
    int premier = 1;
    ll i = 2;
    // n / 2 : division ENTIERE car n et 2 sont des entiers (7/2 = 3), c'est le "div" du pseudo-code
    while (i <= n / 2 && premier == 1) {
        if (n % i == 0) premier = 0;
        else i = i + 1;
    }
    return premier;
}

/* ---------- Algorithme A3 : n pair elimine, puis i impair de 3 a n-2 ---------- */
//
// Idee : un nombre pair (sauf 2) n'est pas premier ; et un nombre impair n'a que des diviseurs
// impairs, donc on teste seulement 3, 5, 7, ... (pas de 2) : environ 2 fois moins de tours.
int A3(ll n) {
    int premier;
    // ll 
    ll i;
    if (n != 2) {                       // != = different de. 2 est le seul nombre pair premier
        if (n % 2 == 0) premier = 0;    // n pair et != 2 : non premier, sans aucune boucle
        else {
            premier = 1;                // n impair : on suppose premier
            i = 3;                      // premier diviseur impair candidat
            while (i <= n - 2 && premier == 1) {
                if (n % i == 0) premier = 0;
                else i = i + 2;         // pas de 2 : 3, 5, 7, 9, ... (impairs uniquement)
            }
        }
    } else premier = 1;                 // n == 2 : premier
    return premier;                     // (premier est affecte dans TOUTES les branches avant ce return)
}

/* ---------- Algorithme A4 : n pair elimine, puis i impair de 3 a n div 2 ---------- */
// A3 avec la borne n/2 au lieu de n-2 : cumule les deux optimisations (impairs + moitie).
int A4(ll n) {
    int premier;
    ll i;
    if (n != 2) {
        if (n % 2 == 0) premier = 0;
        else {
            premier = 1;
            i = 3;
            while (i <= n / 2 && premier == 1) {
                if (n % i == 0) premier = 0;
                else i = i + 2;
            }
        }
    } else premier = 1;
    return premier;
}

/* ---------- Algorithme A5 : i de 2 a sqrt(n) ---------- */
// (copie de lyna.md : cette fonction manquait dans le fichier alors que main l'utilise)
// Si n = a*b, l'un des facteurs est <= racine(n) : inutile de tester au-dela. Complexite O(racine(n)).
int A5(ll n) {
    int premier = 1;
    ll i = 2;
    // sqrt travaille sur des double (reels) : cast (double) avant, cast (ll) apres (troncature).
    // Calcule une seule fois, hors de la boucle (sinon on le recalculerait a chaque tour).
    ll r = (ll) sqrt((double) n);   /* calcule une seule fois, hors de la boucle */
    while (i <= r && premier == 1) {
        if (n % i == 0) premier = 0;
        else i = i + 1;
    }
    return premier;
}

/* ---------- Algorithme A6 : n pair elimine, puis i impair de 3 a sqrt(n) ---------- */
// A5 + elimination des pairs : le plus rapide des six.
int A6(ll n) {
    int premier;
    ll i;
    if (n != 2) {
        if (n % 2 == 0) premier = 0;
        else {
            premier = 1;
            i = 3;
            ll r = (ll) sqrt((double) n);
            while (i <= r && premier == 1) {
                if (n % i == 0) premier = 0;
                else i = i + 2;
            }
        }
    } else premier = 1;
    return premier;
}

/* Repete l'appel jusqu'a 0.05 s au total, puis renvoie le temps moyen (en s) */
// Pourquoi repeter : clock() a une resolution d'environ 1 ms. Un seul appel d'un algo rapide dure
// quelques nanosecondes : impossible a mesurer. On repete donc l'appel et on divise par le nombre de tours.
// int (*f)(ll) = POINTEUR DE FONCTION : f contient l'adresse d'une fonction qui prend un ll et
// renvoie un int. Les parentheses autour de *f sont obligatoires. Cela permet d'utiliser la meme
// fonction mesurer pour A1..A6 : mesurer(A3, n) (le nom d'une fonction = son adresse).
double mesurer(int (*f)(ll), ll n) {
    // volatile : le compilateur ne peut pas supprimer l'appel f(n) sous pretexte que res est inutilise
    volatile int res = 0;
    long reps = 0;          // nombre de repetitions effectuees
    double el;              // temps ecoule (elapsed), en secondes
    clock_t deb = clock();  // temps CPU au depart (en "ticks"; type clock_t)
    // do { } while (cond); : le corps s'execute AU MOINS une fois, puis tant que cond est vraie
    do {
        res = f(n);         // appel de l'algorithme a mesurer via le pointeur de fonction
        reps++;             // reps++ equivaut a reps = reps + 1
        // (double) evite une division entiere (qui donnerait 0). CLOCKS_PER_SEC = ticks par seconde
        el = (double)(clock() - deb) / CLOCKS_PER_SEC;
    } while (el < 0.05);
    (void) res;             // "utilise" res pour eviter l'avertissement "variable non utilisee"
    return el / reps;       // temps moyen d'UN appel
}

// Compte les chiffres decimaux de n : on divise par 10 jusqu'a obtenir 0.
// (n est une copie : le n de l'appelant n'est pas modifie)
int nbChiffres(ll n) {
    int k = 0;
    while (n > 0) { k++; n /= 10; }   // n /= 10 equivaut a n = n / 10
    return k;
}

// main = point d'entree du programme, appele par le systeme.
// argc (argument count) = nombre d'arguments de la ligne de commande, nom du programme compris.
// argv (argument vector) = tableau de chaines : pour "tp1.exe Test-3.txt", argv[0]="tp1.exe", argv[1]="Test-3.txt".
// La valeur renvoyee est le code de sortie : 0 = succes, autre = erreur.
int main(int argc, char *argv[]) {
    if (argc < 2) {                       // aucun fichier donne : on explique l'usage
        printf("Usage : tp1.exe fichier.txt\n");   // \n = retour a la ligne
        return 1;
    }
    // FILE * = pointeur vers la structure representant un fichier ouvert. "r" = lecture (read).
    FILE *f = fopen(argv[1], "r");
    if (f == NULL) {                      // fopen renvoie NULL en cas d'echec : toujours verifier
        printf("Fichier %s introuvable\n", argv[1]);   // %s = format d'une chaine
        return 1;
    }

    // Tableau de 6 pointeurs de fonctions : algo[0]=A1 ... algo[5]=A6. Permet de boucler sur les algos.
    int (*algo[6])(ll) = {A1, A2, A3, A4, A5, A6};

    // En-tete du CSV (separateur ';' : format lu par Excel en version francaise)
    printf("N;chiffres;premier;T_A1;T_A2;T_A3;T_A4;T_A5;T_A6\n");

    ll n;
    // fscanf lit dans le fichier selon un format (%lld = long long) et renvoie le nombre de valeurs lues.
    // &n = ADRESSE de n : fscanf doit pouvoir modifier n. A la fin du fichier elle ne renvoie plus 1 : la boucle s'arrete.
    while (fscanf(f, "%lld", &n) == 1) {
        // debut de ligne : N, nombre de chiffres, est-il premier (A5 : rapide et fiable). %d = int
        printf("%lld;%d;%d", n, nbChiffres(n), A5(n));
        // for (init; condition; pas) : k parcourt 0..5, soit les 6 algorithmes
        for (int k = 0; k < 6; k++) {
            double t = -1;                          /* -1 = non mesure */
            if (k >= 4 || n <= LIMITE_LINEAIRE)     /* A5 et A6 : toujours mesures */
                t = mesurer(algo[k], n);            // || = OU logique ; algo[k] = k-ieme algorithme
            printf(";%.9f", t);                     // %.9f = reel avec 9 decimales (precision a la nanoseconde)
        }
        printf("\n");
        // stdout est "bufferise" (texte garde en memoire puis ecrit par paquets) : fflush force
        // l'ecriture immediate, donc en cas d'interruption les lignes deja calculees sont dans le CSV
        fflush(stdout);
    }
    fclose(f);   // ferme le fichier (libere la ressource)
    return 0;
}
