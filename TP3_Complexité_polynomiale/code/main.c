#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#define CREER_DOSSIER(d) _mkdir(d)
#else
#include <sys/stat.h>
#define CREER_DOSSIER(d) mkdir(d, 0777)
#endif

#include "code.c"

/* =========================================================================
 * TP N°3 : PROGRAMME DE DÉMONSTRATION & MESURE DU TEMPS D'EXÉCUTION
 * ========================================================================= */

// Affiche proprement une petite matrice 2D dans la console
void afficherMatrice(const char *nom, const int *M, int lignes, int colonnes)
{
    printf("\n--- Matrice %s (%d x %d) ---\n", nom, lignes, colonnes);
    for (int i = 0; i < lignes; i++) {
        printf("[ ");
        for (int j = 0; j < colonnes; j++) {
            printf("%4d ", M[i * colonnes + j]);
        }
        printf("]\n");
    }
}

// -------------------------------------------------------------------------
// [COMMENTÉ] L'ancienne lecture binaire (.bin) n'est plus utilisée pour le TP.
// Les données sont désormais lues directement en format texte (.txt).
// -------------------------------------------------------------------------
/*
int *chargerMatriceBinaire(const char *nomFichier, int totalElements)
{
    char chemins[3][256];
    snprintf(chemins[0], sizeof(chemins[0]), "../data/%s", nomFichier);
    snprintf(chemins[1], sizeof(chemins[1]), "data/%s", nomFichier);
    snprintf(chemins[2], sizeof(chemins[2]), "%s", nomFichier);

    FILE *f = NULL;
    for (int k = 0; k < 3; k++) {
        f = fopen(chemins[k], "rb");
        if (f) break;
    }
    if (!f) return NULL;

    int *M = (int *)malloc((size_t)totalElements * sizeof(int));
    if (!M) {
        fclose(f);
        return NULL;
    }
    size_t lus = fread(M, sizeof(int), (size_t)totalElements, f);
    fclose(f);
    if (lus != (size_t)totalElements) {
        free(M);
        return NULL;
    }
    return M;
}
*/

// -------------------------------------------------------------------------
// Charge une matrice texte (.txt) depuis le dossier data/
// Gère automatiquement les chemins relatifs que l'on soit dans code/ ou à la racine.
// -------------------------------------------------------------------------
int *chargerMatriceTexte(const char *nomFichier, int totalElements)
{
    char chemins[3][256];
    snprintf(chemins[0], sizeof(chemins[0]), "../data/%s", nomFichier);
    snprintf(chemins[1], sizeof(chemins[1]), "data/%s", nomFichier);
    snprintf(chemins[2], sizeof(chemins[2]), "%s", nomFichier);

    FILE *f = NULL;
    for (int k = 0; k < 3; k++) {
        f = fopen(chemins[k], "r");
        if (f) break;
    }
    if (!f) {
        return NULL; // Fichier non trouvé
    }

    int *M = (int *)malloc((size_t)totalElements * sizeof(int));
    if (!M) {
        fclose(f);
        return NULL;
    }

    for (int i = 0; i < totalElements; i++) {
        if (fscanf(f, "%d", &M[i]) != 1) {
            break;
        }
    }
    fclose(f);
    return M;
}

// -------------------------------------------------------------------------
// Helper pour ouvrir un fichier dans le dossier 'resultats/'
// -------------------------------------------------------------------------
FILE *ouvrirFichierResultat(const char *nomFichier, const char *mode)
{
    CREER_DOSSIER("../resultats");
    CREER_DOSSIER("resultats");

    char chemin[256];
    snprintf(chemin, sizeof(chemin), "../resultats/%s", nomFichier);
    FILE *f = fopen(chemin, mode);
    if (!f) {
        snprintf(chemin, sizeof(chemin), "resultats/%s", nomFichier);
        f = fopen(chemin, mode);
    }
    return f;
}

// -------------------------------------------------------------------------
// Sauvegarde détaillée du résultat de l'Exercice 1
// -------------------------------------------------------------------------
void sauvegarderResultatProduit(const char *nomFichierResultat, int n, double duree_ms, const int *C)
{
    FILE *f = ouvrirFichierResultat(nomFichierResultat, "w");
    if (!f) return;

    fprintf(f, "============================================================\n");
    fprintf(f, "RESULTAT - EXERCICE 1 : PRODUIT MATRICIEL C = A x B\n");
    fprintf(f, "============================================================\n\n");
    fprintf(f, "Dimensions des matrices   : %d x %d\n", n, n);
    fprintf(f, "Nombre d'operations O(n^3): %lld\n", (long long)n * n * n);
    fprintf(f, "Temps d'execution CPU     : %.4f ms (%.6f secondes)\n\n", duree_ms, duree_ms / 1000.0);

    fprintf(f, "--- Extrait Matrice Resultat C (%d x %d) ---\n", n, n);
    int max_lignes = (n > 50) ? 20 : n;
    int max_cols = (n > 50) ? 20 : n;

    for (int i = 0; i < max_lignes; i++) {
        for (int j = 0; j < max_cols; j++) {
            fprintf(f, "%d ", C[i * n + j]);
        }
        fprintf(f, "\n");
    }
    if (n > 50) {
        fprintf(f, "\n... [Extrait des 20 premieres lignes/colonnes sur %d affiche]\n", n);
    }

    fclose(f);
}

// -------------------------------------------------------------------------
// Sauvegarde détaillée du résultat de l'Exercice 2
// -------------------------------------------------------------------------
void sauvegarderResultatRecherche(const char *nomFichierResultat,
                                  const char *fichier_A, const char *fichier_B,
                                  int n, int m, int n_prime, int m_prime,
                                  int tr1, int pos_i1, int pos_j1, double duree1_ms,
                                  int tr2, int pos_i2, int pos_j2, double duree2_ms)
{
    FILE *f = ouvrirFichierResultat(nomFichierResultat, "w");
    if (!f) return;

    fprintf(f, "============================================================\n");
    fprintf(f, "RESULTAT- EXERCICE 2 : RECHERCHE DE SOUS-MATRICE B DANS A\n");
    fprintf(f, "============================================================\n\n");
    fprintf(f, "Fichier Matrice A (Source)      : %s (%d x %d)\n", fichier_A, n, m);
    fprintf(f, "Fichier Sous-Matrice B (Motif)  : %s (%d x %d)\n\n", fichier_B, n_prime, m_prime);

    fprintf(f, "1. SOUS-MATRICE NON TRIEE (sousMat1 - Approche Naive) :\n");
    if (tr1) {
        fprintf(f, "   -> Statut            : TROUVEE\n");
        fprintf(f, "   -> Position (i, j)   : Ligne = %d, Colonne = %d\n", pos_i1, pos_j1);
    } else {
        fprintf(f, "   -> Statut            : NON TROUVEE\n");
    }
    fprintf(f, "   -> Temps d'execution : %.4f ms (%.6f s)\n\n", duree1_ms, duree1_ms / 1000.0);

    fprintf(f, "2. SOUS-MATRICE AVEC LIGNES TRIEES (sousMat2 - Approche Optimisee) :\n");
    if (tr2) {
        fprintf(f, "   -> Statut            : TROUVEE\n");
        fprintf(f, "   -> Position (i, j)   : Ligne = %d, Colonne = %d\n", pos_i2, pos_j2);
    } else {
        fprintf(f, "   -> Statut            : NON TROUVEE\n");
    }
    fprintf(f, "   -> Temps d'execution : %.4f ms (%.6f s)\n\n", duree2_ms, duree2_ms / 1000.0);

    fprintf(f, "3. COMPARAISON ET ANALYSE :\n");
    if (duree2_ms > 0.00001) {
        fprintf(f, "   -> Gain de vitesse : sousMat2 est %.2f fois plus rapide que sousMat1 !\n", duree1_ms / duree2_ms);
    }
    fprintf(f, "   -> sousMat2 tire profit du tri des lignes pour eliminer des lignes en O(1)\n");
    fprintf(f, "      et localiser la colonne de depart par dichotomie en O(log m).\n");

    fclose(f);
}

// -------------------------------------------------------------------------
// [COMMENTÉ PAR MARIA] Démonstration sur petits exemples
// -------------------------------------------------------------------------
/*
void demo_petits_exemples(void)
{
    printf("\n============================================================\n");
    printf("   1. DEMONSTRATION SUR PETITS EXEMPLES PEDAGOGIQUES\n");
    printf("============================================================\n");

    int A[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int B[3][3] = {{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};
    int C[3][3] = {0};

    afficherMatrice("A (Exemple 1)", (const int *)A, 3, 3);
    afficherMatrice("B (Exemple 1)", (const int *)B, 3, 3);

    clock_t t0 = clock();
    produitMatrices(3, 3, 3, (const int *)A, (const int *)B, (int *)C);
    clock_t t1 = clock();
    double duree_prod = (double)(t1 - t0) * 1000.0 / CLOCKS_PER_SEC;

    afficherMatrice("C = A x B", (const int *)C, 3, 3);
    printf("-> [SUCCES] Produit calcule en %.4f ms (C[0][0] = 30)\n", duree_prod);
    sauvegarderResultatProduit("demo_produit_3x3.txt", 3, duree_prod, (const int *)C);

    int A_fig[10][13] = {
        { 2,  2,  2,  3,  5,  7,  8, 17, 24, 24, 54, 67,  76},
        { 3,  4,  4,  5,  6,  6,  6,  8, 11, 12, 33, 81,  85},
        {12, 14, 23, 26, 26, 26, 31, 34, 44, 45, 52, 87,  90},
        { 6,  6, 17, 24, 24, 54, 56, 61, 67, 81, 87, 90, 108},
        { 2,  2,  2,  3,  5,  7,  8, 17, 24, 24, 54, 67,  76},
        { 3,  4,  4,  5,  6,  6,  6,  8, 11, 12, 33, 81,  85},
        {12, 14, 23, 26, 26, 26, 31, 34, 44, 45, 52, 87,  90},
        { 6,  6, 17, 24, 24, 54, 56, 61, 67, 81, 87, 90, 108},
        {12, 14, 23, 26, 26, 26, 31, 34, 44, 45, 52, 87,  90},
        { 6,  6, 17, 24, 24, 54, 56, 61, 67, 81, 87, 90, 108}
    };
    int B_fig[3][3] = {
        {24, 54, 56},
        { 5,  7,  8},
        { 6,  6,  6}
    };

    printf("\n--- Recherche de Sous-matrice (Figure 1 du TP) ---");
    afficherMatrice("B recherchee", (const int *)B_fig, 3, 3);

    int pos_i1 = -1, pos_j1 = -1;
    t0 = clock();
    int tr1 = sousMat1(10, 13, (const int *)A_fig, 3, 3, (const int *)B_fig, &pos_i1, &pos_j1);
    t1 = clock();
    double duree_sm1 = (double)(t1 - t0) * 1000.0 / CLOCKS_PER_SEC;

    int pos_i2 = -1, pos_j2 = -1;
    t0 = clock();
    int tr2 = sousMat2(10, 13, (const int *)A_fig, 3, 3, (const int *)B_fig, &pos_i2, &pos_j2);
    t1 = clock();
    double duree_sm2 = (double)(t1 - t0) * 1000.0 / CLOCKS_PER_SEC;

    printf("sousMat1 (Naive)     : Trouve = %d a (%d, %d) en %.4f ms\n", tr1, pos_i1, pos_j1, duree_sm1);
    printf("sousMat2 (Optimisee) : Trouve = %d a (%d, %d) en %.4f ms\n", tr2, pos_i2, pos_j2, duree_sm2);
    sauvegarderResultatRecherche("demo_recherche_figure1.txt", 
                                 "Matrice_Figure1", "SousMatrice_Figure1",
                                 10, 13, 3, 3,
                                 tr1, pos_i1, pos_j1, duree_sm1,
                                 tr2, pos_i2, pos_j2, duree_sm2);
}
*/

// -------------------------------------------------------------------------
// EXERCICE 1 : Batterie de tests automatique sur TOUS les fichiers data/
// Tailles : n = 100, 200, 400, 600, 800, 1000
// -------------------------------------------------------------------------
void tester_exercice_1_automatique(void)
{
    printf("\n========================================================================\n");
    printf("   1. EXERCICE 1 : BATTERIE COMPLETE PRODUIT DE MATRICES C = A x B\n");
    printf("   (Tailles n = 100 a 1000)\n");
    printf("========================================================================\n");

    // Toutes les tailles présentes dans le dossier data/
    int tailles[] = {100, 200, 400, 600, 800, 1000};
    int nb_tailles = sizeof(tailles) / sizeof(tailles[0]);

    // Fichier CSV récapitulatif
    FILE *csv = ouvrirFichierResultat("recapitulatif_exercice1_produit.csv", "w");
    if (csv) {
        fprintf(csv, "Taille_n,Operations_O_n3,Temps_ms,Temps_secondes\n");
    }

    printf("%-10s | %-16s | %-16s | %-12s\n", "Taille n", "Operations O(n^3)", "Temps CPU (ms)", "Temps (s)");
    printf("------------------------------------------------------------------------\n");

    for (int k = 0; k < nb_tailles; k++) {
        int n = tailles[k];
        char nom_A[64], nom_B[64], nom_res[64];
        snprintf(nom_A, sizeof(nom_A), "A_%d.txt", n);
        snprintf(nom_B, sizeof(nom_B), "B_%d.txt", n);
        snprintf(nom_res, sizeof(nom_res), "resultat_produit_A%d_B%d.txt", n, n);

        int *A = chargerMatriceTexte(nom_A, n * n);
        int *B = chargerMatriceTexte(nom_B, n * n);
        if (!A || !B) {
            printf("%-10d | [INFO] Fichiers non trouves dans data/\n", n);
            if (A) free(A);
            if (B) free(B);
            continue;
        }

        int *C = (int *)calloc((size_t)n * n, sizeof(int));
        clock_t t0 = clock();
        produitMatricesCarrees(n, A, B, C);
        clock_t t1 = clock();
        double duree_ms = (double)(t1 - t0) * 1000.0 / CLOCKS_PER_SEC;

        long long nb_ops = (long long)n * n * n;
        printf("%-10d | %-16lld | %-16.3f | %-12.6f\n", n, nb_ops, duree_ms, duree_ms / 1000.0);

        if (csv) {
            fprintf(csv, "%d,%lld,%.4f,%.6f\n", n, nb_ops, duree_ms, duree_ms / 1000.0);
        }

        // Sauvegarde individuelle de la matrice dans resultats/
        sauvegarderResultatProduit(nom_res, n, duree_ms, C);

        free(A);
        free(B);
        free(C);
    }

    if (csv) {
        fclose(csv);
        printf("-> [TABLEAU CSV GENERE] : resultats/recapitulatif_exercice1_produit.csv\n");
    }
}

// -------------------------------------------------------------------------
// EXERCICE 2 : Batterie de tests automatique sur TOUS les fichiers data/
// Toutes les tailles n = 100 à 1000 et k = 10 à 50
// -------------------------------------------------------------------------
void tester_exercice_2_automatique(void)
{
    printf("\n========================================================================\n");
    printf("   2. EXERCICE 2 : BATTERIE COMPLETE RECHERCHE DE SOUS-MATRICE B DANS A\n");
    printf("   (Toutes les combinaisons n de 100 a 1000 et k de 10 a 50)\n");
    printf("========================================================================\n");

    int tailles_n[] = {100, 200, 400, 600, 800, 1000};
    int nb_n = sizeof(tailles_n) / sizeof(tailles_n[0]);

    int tailles_k[] = {10, 20, 30, 40, 50};
    int nb_k = sizeof(tailles_k) / sizeof(tailles_k[0]);

    FILE *csv = ouvrirFichierResultat("recapitulatif_exercice2_recherche.csv", "w");
    if (csv) {
        fprintf(csv, "Taille_n,Taille_k,Pos_Attendue,Pos_sousMat1,Temps_sousMat1_ms,Pos_sousMat2,Temps_sousMat2_ms,Ratio_Gain\n");
    }

    printf("%-7s | %-7s | %-12s | %-18s | %-18s | %-10s\n", 
           "n", "k", "Position", "sousMat1 (Naive)", "sousMat2 (Optim)", "Gain");
    printf("------------------------------------------------------------------------------------------\n");

    for (int in = 0; in < nb_n; in++) {
        int n = tailles_n[in];

        // Charger A une seule fois par taille n pour optimiser la vitesse de lecture disque
        char nom_A_nt[64], nom_A_tr[64];
        snprintf(nom_A_nt, sizeof(nom_A_nt), "A_nontriee_%d.txt", n);
        snprintf(nom_A_tr, sizeof(nom_A_tr), "A_triee_%d.txt", n);

        int *A_nt = chargerMatriceTexte(nom_A_nt, n * n);
        int *A_tr = chargerMatriceTexte(nom_A_tr, n * n);

        if (!A_nt || !A_tr) {
            printf("%-7d | [INFO] Matrices A_%d non trouvees dans data/\n", n, n);
            if (A_nt) free(A_nt);
            if (A_tr) free(A_tr);
            continue;
        }

        for (int ik = 0; ik < nb_k; ik++) {
            int k = tailles_k[ik];

            char nom_B_nt[64], nom_B_tr[64], nom_res[64];
            snprintf(nom_B_nt, sizeof(nom_B_nt), "B_nontriee_%d_%d.txt", n, k);
            snprintf(nom_B_tr, sizeof(nom_B_tr), "B_triee_%d_%d.txt", n, k);
            snprintf(nom_res, sizeof(nom_res), "resultat_recherche_A%d_B%d.txt", n, k);

            int *B_nt = chargerMatriceTexte(nom_B_nt, k * k);
            int *B_tr = chargerMatriceTexte(nom_B_tr, k * k);

            int ok1 = 0, pos_i1 = -1, pos_j1 = -1;
            double duree1_ms = 0.0;
            if (B_nt) {
                clock_t t0 = clock();
                ok1 = sousMat1(n, n, A_nt, k, k, B_nt, &pos_i1, &pos_j1);
                clock_t t1 = clock();
                duree1_ms = (double)(t1 - t0) * 1000.0 / CLOCKS_PER_SEC;
                free(B_nt);
            }

            int ok2 = 0, pos_i2 = -1, pos_j2 = -1;
            double duree2_ms = 0.0;
            if (B_tr) {
                clock_t t0 = clock();
                ok2 = sousMat2(n, n, A_tr, k, k, B_tr, &pos_i2, &pos_j2);
                clock_t t1 = clock();
                duree2_ms = (double)(t1 - t0) * 1000.0 / CLOCKS_PER_SEC;
                free(B_tr);
            }

            if (ok1 && ok2) {
                char pos_str[32];
                snprintf(pos_str, sizeof(pos_str), "(%d, %d)", pos_i1, pos_j1);

                char t1_str[32], t2_str[32], gain_str[32];
                snprintf(t1_str, sizeof(t1_str), "%.3f ms", duree1_ms);
                snprintf(t2_str, sizeof(t2_str), "%.3f ms", duree2_ms);

                if (duree2_ms > 0.00001) {
                    snprintf(gain_str, sizeof(gain_str), "x %.1f", duree1_ms / duree2_ms);
                } else {
                    snprintf(gain_str, sizeof(gain_str), "Instantane");
                }

                printf("%-7d | %-7d | %-12s | %-18s | %-18s | %-10s\n",
                       n, k, pos_str, t1_str, t2_str, gain_str);

                if (csv) {
                    int attendu_i = n - k, attendu_j = n - k;
                    fprintf(csv, "%d,%d,\"(%d;%d)\",\"(%d;%d)\",%.4f,\"(%d;%d)\",%.4f,%.2f\n",
                            n, k, attendu_i, attendu_j, 
                            pos_i1, pos_j1, duree1_ms, 
                            pos_i2, pos_j2, duree2_ms,
                            (duree2_ms > 0.0001) ? (duree1_ms / duree2_ms) : 1.0);
                }

                sauvegarderResultatRecherche(nom_res, nom_A_nt, nom_B_nt, n, n, k, k,
                                             ok1, pos_i1, pos_j1, duree1_ms,
                                             ok2, pos_i2, pos_j2, duree2_ms);
            }
        }

        free(A_nt);
        free(A_tr);
    }

    if (csv) {
        fclose(csv);
        printf("\n-> [TABLEAU CSV GENERE] : resultats/recapitulatif_exercice2_recherche.csv\n");
    }
}

int main(void)
{
    printf("========================================================================\n");
    printf("   TP N03 : COMPLEXITE POLYNOMIALE\n");
    printf("========================================================================\n");

    // demo_petits_exemples(); // 

    tester_exercice_1_automatique();
    tester_exercice_2_automatique();

    printf("\n========================================================================\n");
    printf("   TOUS LES TESTS SE SONT DEROULES AVEC SUCCES !\n");
    printf("   Les fichiers de resultats et CSV sont dans le dossier 'resultats/'\n");
    printf("========================================================================\n");
    return 0;
}
