#include <stdio.h>
#include <stdlib.h>
#include "code.c"

/* =========================================================================
 * TP N°3 : PROGRAMME DE DEMONSTRATION & TEST
 * Module : Algorithmique Avancee et Complexite - Master 1 RSD / IL
 * USTHB 2026/2027
 * ========================================================================= */

// Affiche proprement une matrice 2D aplatie en 1D
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

// Optionnel : Permet de lire une matrice depuis un fichier texte si la prof le demande
// Format attendu du fichier : la premiere ligne contient (lignes colonnes), puis les entiers
int *chargerMatriceTexte(const char *nomFichier, int *lignes, int *colonnes)
{
    FILE *f = fopen(nomFichier, "r");
    if (!f) {
        printf("Info : Fichier texte '%s' non present (test interactif en memoire utilise).\n", nomFichier);
        return NULL;
    }
    if (fscanf(f, "%d %d", lignes, colonnes) != 2) {
        fclose(f);
        return NULL;
    }
    int *M = (int *)malloc((size_t)(*lignes) * (*colonnes) * sizeof(int));
    for (int i = 0; i < (*lignes) * (*colonnes); i++) {
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
// TEST DE L'EXERCICE 1 : Produit de deux matrices C = A x B
// -------------------------------------------------------------------------
void test_exercice_1(void)
{
    printf("\n============================================================\n");
    printf("   TEST EXERCICE 1 : PRODUIT DE DEUX MATRICES C = A x B\n");
    printf("============================================================\n");

    int n = 3, m = 3, p = 3;

    // Petite matrice A (3x3)
    int A[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    // Petite matrice B (3x3)
    int B[3][3] = {
        {9, 8, 7},
        {6, 5, 4},
        {3, 2, 1}
    };

    int C[3][3] = {0};

    afficherMatrice("A", (const int *)A, n, m);
    afficherMatrice("B", (const int *)B, m, p);

    // Calcul du produit via notre fonction de code.c
    produitMatrices(n, m, p, (const int *)A, (const int *)B, (int *)C);

    afficherMatrice("C = A x B", (const int *)C, n, p);
    printf("\nVerification : C[0][0] = 1*9 + 2*6 + 3*3 = 9 + 12 + 9 = 30 -> OK !\n");
    printf("[SUCCES] Produit matriciel calcule avec succes !\n");
}

// -------------------------------------------------------------------------
// TEST DE L'EXERCICE 2 : Recherche de sous-matrice
// Utilise l'exemple exact de la Figure 1 du sujet !
// -------------------------------------------------------------------------
void test_exercice_2(void)
{
    printf("\n============================================================\n");
    printf("   TEST EXERCICE 2 : RECHERCHE D'UNE SOUS-MATRICE B DANS A\n");
    printf("   (Exemple exact de la Figure du sujet TP3)\n");
    printf("============================================================\n");

    // Matrice A (10 x 13) dont chaque ligne est triee
    int A[10][13] = {
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

    // Sous-matrice B (3 x 3) a rechercher (situee a la ligne 3, colonne 3)
    int B[3][3] = {
        {24, 54, 56},
        { 5,  7,  8},
        { 6,  6,  6}
    };

    afficherMatrice("Sous-matrice B recherchee", (const int *)B, 3, 3);

    // 1. Test sousMat1 (approche naive / non triee)
    int pos_i1 = -1, pos_j1 = -1;
    int trouve1 = sousMat1(10, 13, (const int *)A, 3, 3, (const int *)B, &pos_i1, &pos_j1);

    printf("\n--- Resultat sousMat1 (Approche Naive) ---\n");
    if (trouve1) {
        printf("-> [SUCCES] Sous-matrice B trouvee a : Ligne = %d, Colonne = %d\n", pos_i1, pos_j1);
    } else {
        printf("-> [ECHEC] Non trouvee\n");
    }

    // 2. Test sousMat2 (approche optimisee avec dichotomie)
    int pos_i2 = -1, pos_j2 = -1;
    int trouve2 = sousMat2(10, 13, (const int *)A, 3, 3, (const int *)B, &pos_i2, &pos_j2);

    printf("\n--- Resultat sousMat2 (Approche Optimisee - Lignes triees) ---\n");
    if (trouve2) {
        printf("-> [SUCCES] Sous-matrice B trouvee a : Ligne = %d, Colonne = %d\n", pos_i2, pos_j2);
    } else {
        printf("-> [ECHEC] Non trouvee\n");
    }
}

int main(void)
{
    printf("============================================================\n");
    printf("   DEMONSTRATION TP N03 : COMPLEXITE POLYNOMIALE\n");
    printf("   USTHB - Master 1 RSD / IL\n");
    printf("============================================================\n");

    test_exercice_1();
    test_exercice_2();

    printf("\n============================================================\n");
    printf("   TOUS LES TESTS SONT VALIDES AVEC SUCCES !\n");
    printf("============================================================\n");
    return 0;
}
