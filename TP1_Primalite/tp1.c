#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef long long ll;

/* Au-dela de cette valeur, les algorithmes en O(n) sont trop lents : on les saute */
// define est une constante de compilation, on peut la modifier avec -D LIMITE_LINEAIRE=... lors de la compilation
// ll est un type entier long long, qui peut contenir des valeurs plus grandes que int ou long
// cette constante est specialement utile pour les algorithmes A1, A2, A3 et A4, qui sont en O(n) et donc trop lents pour de grandes valeurs de n
#define LIMITE_LINEAIRE 1000000000LL

/* ---------- Algorithme A1 : i de 2 a n-1 ---------- */
int A1(ll n) {
    int premier = 1;
    // l'intruction volatile empeche le compilateur d'optimiser la variable i, ce qui est important pour mesurer correctement le temps d'execution
    // ll i = 2;  // on peut aussi utiliser "volatile ll i = 2;" pour la meme raison
    // la volatilite est simplement une indication pour le compilateur que la variable peut changer de maniere inattendue, donc il ne doit pas optimiser son utilisation
    // par optimisation je veux dire que le compilateur pourrait, par exemple, remplacer la variable i par une valeur constante si il pense que i ne change pas, ce qui fausserait la mesure du temps d'execution
    ll i = 2;
    while (i <= n - 1 && premier == 1) {
        if (n % i == 0) premier = 0;
        else i = i + 1;
    }
    return premier;
}

/* ---------- Algorithme A2 : i de 2 a n div 2 ---------- */
int A2(ll n) {
    int premier = 1;
    ll i = 2;
    while (i <= n / 2 && premier == 1) {
        if (n % i == 0) premier = 0;
        else i = i + 1;
    }
    return premier;
}

/* ---------- Algorithme A3 : n pair elimine, puis i impair de 3 a n-2 ---------- */
//
int A3(ll n) {
    int premier;
    // ll 
    ll i;
    if (n != 2) {
        if (n % 2 == 0) premier = 0;
        else {
            premier = 1;
            i = 3;
            while (i <= n - 2 && premier == 1) {
                if (n % i == 0) premier = 0;
                else i = i + 2;
            }
        }
    } else premier = 1;
    return premier;
}

/* ---------- Algorithme A4 : n pair elimine, puis i impair de 3 a n div 2 ---------- */
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



/* Repete l'appel jusqu'a 0.05 s au total, puis renvoie le temps moyen (en s) */
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

int nbChiffres(ll n) {
    int k = 0;
    while (n > 0) { k++; n /= 10; }
    return k;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage : tp1.exe fichier.txt\n");
        return 1;
    }
    FILE *f = fopen(argv[1], "r");
    if (f == NULL) {
        printf("Fichier %s introuvable\n", argv[1]);
        return 1;
    }

    int (*algo[6])(ll) = {A1, A2, A3, A4, A5, A6};

    printf("N;chiffres;premier;T_A1;T_A2;T_A3;T_A4;T_A5;T_A6\n");

    ll n;
    while (fscanf(f, "%lld", &n) == 1) {
        printf("%lld;%d;%d", n, nbChiffres(n), A5(n));
        for (int k = 0; k < 6; k++) {
            double t = -1;                          /* -1 = non mesure */
            if (k >= 4 || n <= LIMITE_LINEAIRE)     /* A5 et A6 : toujours mesures */
                t = mesurer(algo[k], n);
            printf(";%.9f", t);
        }
        printf("\n");
        fflush(stdout);
    }
    fclose(f);
    return 0;
}
