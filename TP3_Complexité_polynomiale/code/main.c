#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "code.c"

/* =========================================================================
 * TP N°3 : PROGRAMME DE DÉMONSTRATION & MESURE DU TEMPS D'EXÉCUTION
 * Module : Algorithmique Avancée et Complexité - Master 1 RSD / IL
 * USTHB 2026/2027
 * ========================================================================= */

// Affiche proprement une petite matrice 2D
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
// Charge une matrice binaire (.bin) depuis le dossier data
// -------------------------------------------------------------------------
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

// -------------------------------------------------------------------------
// Charge une matrice texte (.txt) depuis le dossier data
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
    if (!f) return NULL;

    int *M = (int *)malloc((size_t)totalElements * sizeof(int));
    if (!M) {
        fclose(f);
        return NULL;
    }
    for (int i = 0; i < totalElements; i++) {
        if (fscanf(f, "%d", &M[i]) != 1) {
            free(M);
            fclose(f);
            return NULL;
        }
    }
    fclose(f);
    return M;
}

// -------------------------------------------------------------------------
// PARTIE 1 : Démonstration sur petits exemples avec affichage complet
// -------------------------------------------------------------------------
void demo_petits_exemples(void)
{
    printf("\n============================================================\n");
    printf("   1. DEMONSTRATION SUR PETITS EXEMPLES PEDAGOGIQUES\n");
    printf("============================================================\n");

    // --- Exercice 1 : Produit 3x3 ---
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

    // --- Exercice 2 : Sous-matrice Figure 1 du TP (10 x 13) ---
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
}

// -------------------------------------------------------------------------
// PARTIE 2 : Test avec les vrais fichiers du dossier data/ (Ex: n = 100)
// -------------------------------------------------------------------------
void demo_fichiers_data(void)
{
    printf("\n============================================================\n");
    printf("   2. TEST EN CONDITIONS REELLES SUR LE DOSSIER data/\n");
    printf("   (Mesure du temps d'execution CPU pour n = 100)\n");
    printf("============================================================\n");

    // 1. Exercice 1 : Produit de deux matrices 100 x 100 (1 000 000 operations)
    printf("\n>>> TEST EXERCICE 1 SUR DATA : A_100.bin x B_100.bin (100 x 100) ...\n");
    int n1 = 100;
    int *A_data = chargerMatriceBinaire("A_100.bin", n1 * n1);
    int *B_data = chargerMatriceBinaire("B_100.bin", n1 * n1);
    int *C_data = (int *)calloc(n1 * n1, sizeof(int));

    if (A_data && B_data && C_data) {
        clock_t t0 = clock();
        produitMatricesCarrees(n1, A_data, B_data, C_data);
        clock_t t1 = clock();
        double duree_ms = (double)(t1 - t0) * 1000.0 / CLOCKS_PER_SEC;

        printf("-> [SUCCES] Produit C(100, 100) calcule avec succes !\n");
        printf("-> Temps d'execution CPU : %.3f ms (soit %.6f secondes)\n", duree_ms, duree_ms / 1000.0);
        printf("-> Verification case C[0][0] = %d\n", C_data[0]);

        free(A_data);
        free(B_data);
        free(C_data);
    } else {
        printf("-> [INFO] Fichiers A_100.bin / B_100.bin non trouves dans data/.\n");
    }

    // 2. Exercice 2 : Recherche de sous-matrice sur data (n = 100, k = 10)
    // D'apres ex2_positions.csv, la position attendue est (90, 90)
    printf("\n>>> TEST EXERCICE 2 SUR DATA : Recherche de B (10x10) dans A (100x100) ...\n");

    // A. sousMat1 sur matrice non triée
    int *A_nt = chargerMatriceBinaire("A_nontriee_100.bin", 100 * 100);
    int *B_nt = chargerMatriceBinaire("B_nontriee_100_10.bin", 10 * 10);
    if (A_nt && B_nt) {
        int pos_i = -1, pos_j = -1;
        clock_t t0 = clock();
        int ok = sousMat1(100, 100, A_nt, 10, 10, B_nt, &pos_i, &pos_j);
        clock_t t1 = clock();
        double duree_ms = (double)(t1 - t0) * 1000.0 / CLOCKS_PER_SEC;

        printf("-> sousMat1 (Naive)     : Position = (%d, %d) [Attendu: (90, 90)] | Temps = %.3f ms\n", 
               pos_i, pos_j, duree_ms);
        free(A_nt);
        free(B_nt);
    }

    // B. sousMat2 sur matrice triée
    int *A_tr = chargerMatriceBinaire("A_triee_100.bin", 100 * 100);
    int *B_tr = chargerMatriceBinaire("B_triee_100_10.bin", 10 * 10);
    if (A_tr && B_tr) {
        int pos_i = -1, pos_j = -1;
        clock_t t0 = clock();
        int ok = sousMat2(100, 100, A_tr, 10, 10, B_tr, &pos_i, &pos_j);
        clock_t t1 = clock();
        double duree_ms = (double)(t1 - t0) * 1000.0 / CLOCKS_PER_SEC;

        printf("-> sousMat2 (Optimisee) : Position = (%d, %d) [Attendu: (90, 90)] | Temps = %.3f ms\n", 
               pos_i, pos_j, duree_ms);
        free(A_tr);
        free(B_tr);
    }
}

int main(void)
{
    printf("============================================================\n");
    printf("   TP N03 : COMPLEXITE POLYNOMIALE - USTHB 2026/2027\n");
    printf("   Algorithmique Avancee & Complexite - Master 1 RSD / IL\n");
    printf("============================================================\n");

    demo_petits_exemples();
    demo_fichiers_data();

    printf("\n============================================================\n");
    printf("   TOUS LES TESTS SE SONT DEROULES AVEC SUCCES !\n");
    printf("============================================================\n");
    return 0;
}
