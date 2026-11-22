/*
 * TP2 - MESURE DES TEMPS D'EXECUTION
 *
 * Lit les fichiers de ../data (non_trie_<n>.txt et trie_<n>.txt), mesure le temps
 * de chaque fonction et ecrit deux fichiers CSV dans le dossier courant :
 *   resultats_A.csv : temps (microsecondes) des 3 recherches, meilleur et pire cas
 *   resultats_B.csv : temps (ms) et nombre de comparaisons de MaxEtMinA / MaxEtMinB
 *
 * Compilation (depuis le dossier tests/) :  gcc -O0 mesures.c -o mesures
 *   (-O0 = pas d'optimisation du compilateur, pour que les mesures refletent le code ecrit)
 * Execution :  ./mesures   (mesures.exe sous Windows)
 * Puis      :  python graphiques.py
 *
 * Cas mesures :
 *   NonTries : meilleur = x en 1re case          | pire = x absent (parcours complet)
 *   Tries    : meilleur = x en 1re case          | pire = x plus grand que tout (parcours complet)
 *   Dicho    : meilleur = x au milieu            | pire = x absent
 */
#define main main_code
#include "../code/Code"
#undef main

static const int TAILLES[] = {100000, 200000, 400000, 600000, 800000,
                              1000000, 1200000, 1400000, 1600000, 1800000,
                              2000000, 4000000, 5000000, 6000000, 7000000, 8000000};
#define NB_TAILLES ((int)(sizeof(TAILLES) / sizeof(TAILLES[0])))

static volatile long puits;   /* empeche le compilateur de supprimer les appels mesures */

/* Lit exactement n entiers dans ../data/<prefixe>_<n>.txt ; NULL si absent */
static int *charger(const char *prefixe, int n) {
    char nom[256];
    sprintf(nom, "../data/%s_%d.txt", prefixe, n);
    FILE *f = fopen(nom, "r");
    if (!f) return NULL;
    int *t = malloc((size_t)n * sizeof(int));
    int lu = 0, v;
    while (lu < n && fscanf(f, "%d", &v) == 1) t[lu++] = v;
    fclose(f);
    if (lu != n) { free(t); return NULL; }
    return t;
}

/* Temps moyen d'UNE recherche, en microsecondes (moyenne sur "rep" repetitions) */
static double mesure_recherche(int (*f)(int[], int, int), int T[], int n, int x, long rep) {
    long s = 0;
    clock_t debut = clock();
    for (long k = 0; k < rep; k++) s += f(T, n, x);
    clock_t fin = clock();
    puits = s;
    return ((double)(fin - debut) / CLOCKS_PER_SEC) / rep * 1e6;
}

/* Temps moyen (ms) d'une copie du tableau : a retrancher car MaxEtMinB modifie le tableau */
static double mesure_copie(int *dst, const int *src, int n, int rep) {
    clock_t debut = clock();
    for (int r = 0; r < rep; r++) memcpy(dst, src, (size_t)n * sizeof(int));
    clock_t fin = clock();
    puits = dst[0];
    return ((double)(fin - debut) / CLOCKS_PER_SEC) / rep * 1000.0;
}

int main(void) {
    FILE *fa = fopen("resultats_A.csv", "w");
    FILE *fb = fopen("resultats_B.csv", "w");
    if (!fa || !fb) { printf("Impossible de creer les fichiers CSV\n"); return 1; }

    fprintf(fa, "n,nontrie_meilleur_us,nontrie_pire_us,trie_meilleur_us,trie_pire_us,dicho_meilleur_us,dicho_pire_us\n");
    fprintf(fb, "n,A_temps_ms,A_comparaisons,A_theorique,B_temps_ms,B_comparaisons,B_theorique\n");

    const long REP_O1    = 20000000L;   /* cas en O(1)      : tres rapides */
    const long REP_DICHO = 2000000L;    /* cas en O(log n)  */

    printf("%9s | %12s %12s | %12s %12s | %12s %12s\n", "n",
           "NonTr-meil", "NonTr-pire", "Tries-meil", "Tries-pire", "Dicho-meil", "Dicho-pire");
    printf("(temps en microsecondes pour UNE recherche)\n");

    for (int k = 0; k < NB_TAILLES; k++) {
        int n = TAILLES[k];
        int *nt = charger("non_trie", n);
        int *tr = charger("trie", n);
        if (!nt || !tr) { printf("%9d | fichiers absents, taille ignoree\n", n); free(nt); free(tr); continue; }

        long rep_lin = 300000000L / n;          /* ~3e8 elements parcourus au total */
        if (rep_lin < 5) rep_lin = 5;

        /* ----- Partie A ----- */
        double a1 = mesure_recherche(rechElets_TabNonTries, nt, n, nt[0],        REP_O1);
        double a2 = mesure_recherche(rechElets_TabNonTries, nt, n, -1,           rep_lin);
        double b1 = mesure_recherche(rechElets_TabTries,    tr, n, tr[0],        REP_O1);
        double b2 = mesure_recherche(rechElets_TabTries,    tr, n, tr[n-1] + 1,  rep_lin);
        double c1 = mesure_recherche(rechElets_Dicho,       tr, n, tr[(n-1)/2],  REP_O1);
        double c2 = mesure_recherche(rechElets_Dicho,       tr, n, tr[n-1] + 1,  REP_DICHO);

        fprintf(fa, "%d,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n", n, a1, a2, b1, b2, c1, c2);
        printf("%9d | %12.6f %12.6f | %12.6f %12.6f | %12.6f %12.6f\n", n, a1, a2, b1, b2, c1, c2);
        fflush(stdout);

        /* ----- Partie B (valeurs distinctes = fichier non trie) ----- */
        int *cp = malloc((size_t)n * sizeof(int));
        int rep_b = (int)(300000000L / n / 2);
        if (rep_b < 5) rep_b = 5;
        int maxA, minA, maxB, minB;

        memcpy(cp, nt, (size_t)n * sizeof(int));
        long cptA = MaxEtMinA(cp, n, &maxA, &minA);
        memcpy(cp, nt, (size_t)n * sizeof(int));
        long cptB = MaxEtMinB(cp, n, &maxB, &minB);
        if (maxA != maxB || minA != minB) printf("ERREUR : A et B differents pour n=%d\n", n);

        double copie = mesure_copie(cp, nt, n, rep_b);

        clock_t d = clock();
        for (int r = 0; r < rep_b; r++) { memcpy(cp, nt, (size_t)n * sizeof(int)); MaxEtMinA(cp, n, &maxA, &minA); }
        double tA = (double)(clock() - d) / CLOCKS_PER_SEC / rep_b * 1000.0 - copie;

        d = clock();
        for (int r = 0; r < rep_b; r++) { memcpy(cp, nt, (size_t)n * sizeof(int)); MaxEtMinB(cp, n, &maxB, &minB); }
        double tB = (double)(clock() - d) / CLOCKS_PER_SEC / rep_b * 1000.0 - copie;

        fprintf(fb, "%d,%.6f,%ld,%ld,%.6f,%ld,%ld\n", n, tA, cptA, 2L*(n-1), tB, cptB, (3L*n+1)/2 - 2);
        free(cp); free(nt); free(tr);
    }

    fclose(fa); fclose(fb);
    printf("\nTermine : resultats_A.csv et resultats_B.csv crees dans ce dossier.\n");
    return 0;
}
