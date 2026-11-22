/*
 * TP2 - TESTS DE JUSTESSE (a lancer AVANT de mesurer les temps)
 *
 * Ce fichier inclut directement le code de la personne qui programme
 * (../code/Code). Son main() est renomme pour ne pas entrer en conflit.
 *
 * Compilation (depuis le dossier tests/) :  gcc tests.c -o tests
 * Execution                              :  ./tests
 *
 * Ce que l'on verifie :
 *   1. les 3 fonctions de recherche (premier, dernier, milieu, absent, n=2, n pair/impair)
 *   2. MaxEtMinA et MaxEtMinB donnent le meme resultat
 *   3. le nombre de comparaisons = valeur theorique (2(n-1) et ceil(3n/2)-2)
 *   4. les fichiers de donnees ../data (fichiers .txt) sont corrects (bonne taille, tries, sans doublons)
 */
#define main main_code
#include "../code/Code"
#undef main

static int nb_erreurs = 0;

#define CHECK(cond, msg) do { \
    if (!(cond)) { printf("ECHEC : %s\n", msg); nb_erreurs++; } \
} while (0)

/* ---------- 1. Recherche ---------- */
static void test_recherche(void) {
    int pair[]   = {1, 3, 5, 7, 9, 11};     /* n pair   */
    int impair[] = {1, 3, 5, 7, 9};         /* n impair */
    int deux[]   = {2, 8};                  /* n = 2    */
    int *tabs[] = {pair, impair, deux};
    int  ns[]   = {6, 5, 2};

    for (int k = 0; k < 3; k++) {
        int *t = tabs[k]; int n = ns[k];
        CHECK(rechElets_TabNonTries(t, n, t[0])   == 1, "NonTries : premier");
        CHECK(rechElets_TabNonTries(t, n, t[n-1]) == 1, "NonTries : dernier");
        CHECK(rechElets_TabTries(t, n, t[0])      == 1, "Tries : premier");
        CHECK(rechElets_TabTries(t, n, t[n-1])    == 1, "Tries : dernier");
        CHECK(rechElets_Dicho(t, n, t[0])         == 1, "Dicho : premier");
        CHECK(rechElets_Dicho(t, n, t[n-1])       == 1, "Dicho : dernier");
        CHECK(rechElets_Dicho(t, n, t[(n-1)/2])   == 1, "Dicho : milieu");
        /* absents : trop petit, trop grand, entre deux valeurs */
        CHECK(rechElets_TabNonTries(t, n, -1)     == 0, "NonTries : absent (petit)");
        CHECK(rechElets_TabTries(t, n, -1)        == 0, "Tries : absent (petit)");
        CHECK(rechElets_TabTries(t, n, 1000)      == 0, "Tries : absent (grand)");
        CHECK(rechElets_TabTries(t, n, 4)         == 0, "Tries : absent (entre)");
        CHECK(rechElets_Dicho(t, n, -1)           == 0, "Dicho : absent (petit)");
        CHECK(rechElets_Dicho(t, n, 1000)         == 0, "Dicho : absent (grand)");
        CHECK(rechElets_Dicho(t, n, 4)            == 0, "Dicho : absent (entre)");
    }
    int nt[] = {9, 2, 7, 4, 1};             /* non trie */
    CHECK(rechElets_TabNonTries(nt, 5, 1) == 1, "NonTries : x a la fin");
    CHECK(rechElets_TabNonTries(nt, 5, 5) == 0, "NonTries : x absent");
}

/* ---------- 2. Max et Min : resultats ---------- */
static void test_maxmin(void) {
    int a1[] = {5, 2, 7, 3, 1, 8, 4};       /* n impair : max=8, min=1 */
    int a2[] = {5, 2, 7, 3, 1, 8};          /* n pair   : max=8, min=1 */
    int a3[] = {4, 9};                      /* n = 2    : max=9, min=4 */
    int a4[] = {9, 4};                      /* n = 2 inverse           */
    int *tabs[] = {a1, a2, a3, a4};
    int  ns[]   = {7, 6, 2, 2};
    int  mx[]   = {8, 8, 9, 9};
    int  mn[]   = {1, 1, 4, 4};

    for (int k = 0; k < 4; k++) {
        int max, min, copie[8];
        /* MaxEtMinB modifie le tableau : on travaille sur des copies */
        memcpy(copie, tabs[k], ns[k] * sizeof(int));
        MaxEtMinA(copie, ns[k], &max, &min);
        CHECK(max == mx[k] && min == mn[k], "MaxEtMinA : resultat");
        memcpy(copie, tabs[k], ns[k] * sizeof(int));
        MaxEtMinB(copie, ns[k], &max, &min);
        CHECK(max == mx[k] && min == mn[k], "MaxEtMinB : resultat");
    }
}

/* ---------- 3. Nombre de comparaisons vs theorique ---------- */
static void test_compteur(void) {
    int tailles[] = {2, 3, 10, 11, 1000, 1001};
    for (int k = 0; k < 6; k++) {
        int n = tailles[k];
        int *t  = malloc(n * sizeof(int));
        int *cp = malloc(n * sizeof(int));
        remplirAleatoire(t, n);
        int maxA, minA, maxB, minB;
        memcpy(cp, t, n * sizeof(int));
        long cA = MaxEtMinA(cp, n, &maxA, &minA);
        memcpy(cp, t, n * sizeof(int));
        long cB = MaxEtMinB(cp, n, &maxB, &minB);
        long thA = 2L * (n - 1);
        long thB = (3L * n + 1) / 2 - 2;            /* ceil(3n/2) - 2 */
        printf("n=%4d : A=%ld (theo %ld) | B=%ld (theo %ld)\n", n, cA, thA, cB, thB);
        CHECK(maxA == maxB && minA == minB, "A et B donnent des resultats differents");
        CHECK(cA == thA, "compteur MaxEtMinA different de 2(n-1)");
        CHECK(cB == thB, "compteur MaxEtMinB different de ceil(3n/2)-2");
        free(t); free(cp);
    }
}

/* ---------- 4. Fichiers de donnees ../data (fichiers .txt) ---------- */

/* Lit exactement n entiers (un par ligne). NULL si fichier absent ou taille differente. */
static int *charger(const char *prefixe, int n) {
    char nom[256];
    sprintf(nom, "../data/%s_%d.txt", prefixe, n);
    FILE *f = fopen(nom, "r");
    if (!f) { printf("   (fichier absent : %s)\n", nom); return NULL; }
    int *t = malloc((size_t)n * sizeof(int));
    int lu = 0, v;
    while (lu < n && fscanf(f, "%d", &v) == 1) t[lu++] = v;
    int en_trop = (fscanf(f, "%d", &v) == 1);       /* y a-t-il plus de n valeurs ? */
    fclose(f);
    if (lu != n || en_trop) { printf("ECHEC : %s ne contient pas exactement %d valeurs\n", nom, n); nb_erreurs++; free(t); return NULL; }
    return t;
}

static int cmp_int(const void *a, const void *b) {
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

static void test_fichiers(void) {
    int tailles[] = {100000, 200000, 400000, 600000, 800000,
                     1000000, 1200000, 1400000, 1600000, 1800000,
                     2000000, 4000000, 5000000, 6000000, 7000000, 8000000};
    int nb = sizeof(tailles) / sizeof(tailles[0]);
    int traites = 0;

    for (int k = 0; k < nb; k++) {
        int n = tailles[k];
        int *nt = charger("non_trie", n);
        int *tr = charger("trie", n);
        if (!nt || !tr) { free(nt); free(tr); continue; }
        traites++;

        /* le fichier "trie" est strictement croissant (donc trie + sans doublons) */
        int ok = 1;
        for (int i = 1; i < n; i++) if (tr[i-1] >= tr[i]) { ok = 0; break; }
        CHECK(ok, "fichier trie : pas strictement croissant");

        /* le fichier "non trie" contient les memes valeurs, toutes distinctes */
        int *cp = malloc((size_t)n * sizeof(int));
        memcpy(cp, nt, (size_t)n * sizeof(int));
        qsort(cp, n, sizeof(int), cmp_int);
        CHECK(memcmp(cp, tr, (size_t)n * sizeof(int)) == 0, "non_trie et trie n'ont pas les memes valeurs / doublons");
        free(cp);

        /* les fonctions de recherche sur de vraies donnees (meilleur et pire cas) */
        CHECK(rechElets_TabNonTries(nt, n, nt[0])   == 1, "donnees : NonTries meilleur cas");
        CHECK(rechElets_TabNonTries(nt, n, nt[n-1]) == 1, "donnees : NonTries dernier");
        CHECK(rechElets_TabNonTries(nt, n, -1)      == 0, "donnees : NonTries pire cas (absent)");
        CHECK(rechElets_TabTries(tr, n, tr[0])      == 1, "donnees : Tries meilleur cas");
        CHECK(rechElets_TabTries(tr, n, tr[n-1])    == 1, "donnees : Tries dernier");
        CHECK(rechElets_TabTries(tr, n, tr[n-1]+1)  == 0, "donnees : Tries pire cas (absent)");
        CHECK(rechElets_Dicho(tr, n, tr[(n-1)/2])   == 1, "donnees : Dicho milieu");
        CHECK(rechElets_Dicho(tr, n, tr[0])         == 1, "donnees : Dicho premier");
        CHECK(rechElets_Dicho(tr, n, tr[n-1])       == 1, "donnees : Dicho dernier");
        CHECK(rechElets_Dicho(tr, n, tr[n-1]+1)     == 0, "donnees : Dicho pire cas (absent)");

        /* Partie B sur les donnees : A et B donnent le meme max/min = extremes du fichier trie */
        int maxA, minA, maxB, minB;
        long cA = MaxEtMinA(nt, n, &maxA, &minA);
        long cB = MaxEtMinB(nt, n, &maxB, &minB);   /* modifie nt, ce n'est plus utilise apres */
        CHECK(maxA == tr[n-1] && minA == tr[0], "donnees : MaxEtMinA different des extremes");
        CHECK(maxB == tr[n-1] && minB == tr[0], "donnees : MaxEtMinB different des extremes");
        CHECK(cA == 2L*(n-1), "donnees : compteur A != 2(n-1)");
        CHECK(cB == (3L*n+1)/2 - 2, "donnees : compteur B != ceil(3n/2)-2");

        printf("   n=%8d : fichiers OK\n", n);
        free(nt); free(tr);
    }
    if (traites == 0) { printf("ECHEC : aucun fichier de donnees trouve dans ../data (lancer depuis le dossier tests/)\n"); nb_erreurs++; }
}

int main(void) {
    srand(1);
    printf("== 1. Recherche ==\n");            test_recherche();
    printf("== 2. Max et Min (resultats) ==\n"); test_maxmin();
    printf("== 3. Compteurs de comparaisons ==\n"); test_compteur();
    printf("== 4. Fichiers de donnees ==\n");  test_fichiers();
    if (nb_erreurs == 0) printf("\nTous les tests sont OK.\n");
    else printf("\n%d erreur(s).\n", nb_erreurs);
    return nb_erreurs != 0;
}