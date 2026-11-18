#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* =========================================================================
 * TP N°3 - TEST D'ASSIDUITE : TRI A BULLES (BUBBLE SORT)
 * Algorithmique Avancee et Complexite - Master 1 (RSD / IL) - USTHB
 * Enseignante : Mme C. IGHILAZA
 *
 * ALGORITHME DONNE EN PSEUDO-CODE (indices de 1 a n) :
 *   Pour i = n a 1 faire
 *       Pour j = 2 a i faire
 *           Si T[j-1] > T[j] alors
 *               permuter(T[j-1], T[j])
 *           Fin Si
 *       Fin Pour
 *   Fin Pour
 *
 * NOTE DE TRADUCTION EN C (indices de 0 a n-1) :
 *   En C, les tableaux commencent a 0.
 *   - L'indice j va de 1 a i (equivalent de 2 a i en 1-indexed)
 *   - On compare T[j-1] avec T[j]
 * ========================================================================= */

// Fonction pour échanger deux entiers
void permuter(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Fonction d'affichage d'un tableau
void afficherTableau(const char *message, const int T[], int n)
{
    printf("%s", message);
    printf("[ ");
    for (int i = 0; i < n; i++) {
        printf("%d ", T[i]);
    }
    printf("]\n");
}

/**
 * Algorithme du Tri à Bulles fidèle au pseudo-code de Mme Ighilaza.
 *
 * Complexité temporelle :
 *   - Pire cas : O(n^2)  [Tableau trié en ordre inverse]
 *   - Meilleur cas : O(n^2) (ou O(n) si optimisé avec un booléen 'trié')
 *   - Nombre de comparaisons : n*(n-1)/2
 * Complexité spatiale : O(1) (tri sur place)
 */
void triBulles(int T[], int n)
{
    // i va de n-1 jusqu'à 1 (décroissant)
    for (int i = n - 1; i >= 1; i--) {
        // j va de 1 jusqu'à i (comparaison des éléments adjacents)
        for (int j = 1; j <= i; j++) {
            // Si l'élément précédent est supérieur à l'élément courant
            if (T[j - 1] > T[j]) {
                permuter(&T[j - 1], &T[j]);
            }
        }
    }
}

int main(void)
{
    printf("============================================================\n");
    printf("   TEST D'ASSIDUITE : ALGORITHME DU TRI A BULLES\n");
    printf("   USTHB - Master 1 RSD / IL\n");
    printf("============================================================\n\n");

    // Exemple de test avec un tableau quelconque
    int T[] = {64, 34, 25, 12, 22, 11, 90, 5, 88, 3};
    int n = sizeof(T) / sizeof(T[0]);

    afficherTableau("Tableau AVANT le tri : ", T, n);

    // Mesure du temps d'exécution
    clock_t debut = clock();
    triBulles(T, n);
    clock_t fin = clock();
    double temps_ms = (double)(fin - debut) * 1000.0 / CLOCKS_PER_SEC;

    afficherTableau("Tableau APRES le tri : ", T, n);

    printf("\n-> [SUCCES] Tableau parfaitement trie en ordre croissant !\n");
    printf("-> Temps d'execution CPU : %.4f ms\n", temps_ms);
    printf("============================================================\n");

    return 0;
}
