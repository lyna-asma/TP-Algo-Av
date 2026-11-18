#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* =========================================================================
 * TP N°3 : COMPLEXITÉ POLYNOMIALE
 * Module : Algorithmique Avancée et Complexité - Master 1 (IL & RSD)
 * Enseignante : Mme C. IGHILAZA - USTHB
 * ========================================================================= */

/* =========================================================================
 * EXERCICE 1 : PRODUIT DE DEUX MATRICES
 * Formule : C(n, p) = A(n, m) * B(m, p)
 * C(i, j) = Somme_{k=0..m-1} (A[i][k] * B[k][j])
 *
 * Représentation mémoire :
 * Les matrices sont stockées sous forme de tableaux 1D contigus en mémoire :
 *   A[i][k] correspond à A[i * m + k]
 *   B[k][j] correspond à B[k * p + j]
 *   C[i][j] correspond à C[i * p + j]
 * ========================================================================= */

/**
 * Calcule le produit C = A * B pour des matrices de dimensions quelconques.
 * @param n : nombre de lignes de A (et de C)
 * @param m : nombre de colonnes de A et nombre de lignes de B
 * @param p : nombre de colonnes de B (et de C)
 * @param A : matrice A de taille n x m
 * @param B : matrice B de taille m x p
 * @param C : matrice résultat de taille n x p (allouée par l'appelant)
 *
 * Complexité temporelle théorique : O(n * m * p)
 * Complexité spatiale : O(1) en mémoire auxiliaire (hors matrices d'entrée/sortie)
 */
void produitMatrices(int n, int m, int p, const int *A, const int *B, int *C)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p; j++) {
            int somme = 0;
            for (int k = 0; k < m; k++) {
                somme += A[i * m + k] * B[k * p + j];
            }
            C[i * p + j] = somme;
        }
    }
}

/**
 * Cas particulier : Produit de deux matrices carrées de taille n x n.
 * Complexité temporelle : O(n^3) opérations élémentaires.
 */
void produitMatricesCarrees(int n, const int *A, const int *B, int *C)
{
    produitMatrices(n, n, n, A, B, C);
}


/* =========================================================================
 * EXERCICE 2 : RECHERCHE D'UNE SOUS-MATRICE
 * Soit A(n, m) et B(n', m') avec n' < n et m' < m.
 * Il s'agit de trouver si B apparaît dans A et d'identifier sa position (i, j).
 * ========================================================================= */

/**
 * 1. SOUS-MATRICE DANS UN TABLEAU NON TRIÉ (Approche Naïve)
 * Parcourt toutes les positions possibles (i, j) dans A et teste l'égalité
 * de la fenêtre de taille n' x m' élément par élément.
 *
 * @return 1 si B est trouvée dans A (avec *pos_i et *pos_j mis à jour), 0 sinon.
 *
 * Complexité temporelle théorique :
 *   Pire cas : O((n - n' + 1) * (m - m' + 1) * n' * m')
 */
int sousMat1(int n, int m, const int *A, 
             int n_prime, int m_prime, const int *B, 
             int *pos_i, int *pos_j)
{
    // Limites de parcours pour que la sous-matrice B ne dépasse pas de A
    int max_i = n - n_prime;
    int max_j = m - m_prime;

    for (int i = 0; i <= max_i; i++) {
        for (int j = 0; j <= max_j; j++) {
            
            // Vérification de la correspondance bloc par bloc
            int correspond = 1;
            for (int di = 0; di < n_prime; di++) {
                for (int dj = 0; dj < m_prime; dj++) {
                    if (A[(i + di) * m + (j + dj)] != B[di * m_prime + dj]) {
                        correspond = 0;
                        break; // Arrêt dès la première divergence
                    }
                }
                if (!correspond) break;
            }

            // Si tous les éléments de B correspondent
            if (correspond) {
                if (pos_i) *pos_i = i;
                if (pos_j) *pos_j = j;
                return 1; // Trouvé !
            }
        }
    }
    return 0; // B n'existe pas dans A
}


/**
 * Fonction auxiliaire pour sousMat2 :
 * Recherche dichotomique de la première occurrence d'une valeur cible
 * dans un tableau 1D trié T de taille taille_max.
 *
 * Complexité : O(log2(taille_max))
 */
static int dicho_premiere_occurrence(const int *T, int taille_max, int cible)
{
    int g = 0, d = taille_max - 1;
    int res = -1;
    while (g <= d) {
        int mid = (g + d) / 2;
        if (T[mid] == cible) {
            res = mid;
            d = mid - 1; // Continuer vers la gauche pour trouver la 1ère occurrence
        } else if (T[mid] < cible) {
            g = mid + 1;
        } else {
            d = mid - 1;
        }
    }
    return res;
}

/**
 * 2. SOUS-MATRICE AVEC LIGNES TRIÉES PAR ORDRE CROISSANT (Approche Optimisée)
 * (Proposé par S. Larabi au concours doctorat 2017-2018)
 *
 * Principe d'optimisation :
 * Comme chaque ligne de A et de B est triée en ordre croissant :
 * 1. Élagage immédiat (Pruning) : Pour une ligne i donnée, si B[0][0] est plus petit
 *    que A[i][0] ou si B[0][m'-1] est plus grand que A[i][m-1], la sous-matrice B
 *    ne peut absolument pas commencer à la ligne i. On saute la ligne en O(1) !
 * 2. Recherche dichotomique de la colonne d'ancrage :
 *    Au lieu de tester séquentiellement toutes les colonnes j de 0 à m - m', on cherche
 *    directement la cible B[0][0] sur la ligne A[i] par DICHOTOMIE en O(log2 m) !
 * 3. Vérification ciblée :
 *    On ne compare les éléments complets de B que pour les colonnes candidates trouvées.
 *
 * Complexité temporelle théorique :
 *   Cas moyen / Rejet : O((n - n' + 1) * log2 m)
 *   Pire cas : O((n - n' + 1) * (log2 m + n' * m'))
 */
int sousMat2(int n, int m, const int *A, 
             int n_prime, int m_prime, const int *B, 
             int *pos_i, int *pos_j)
{
    int max_i = n - n_prime;
    int max_j = m - m_prime;
    int cible = B[0]; // Premier élément de B : B[0][0]
    int dernier_B0 = B[m_prime - 1]; // Dernier élément de la 1ère ligne de B

    for (int i = 0; i <= max_i; i++) {
        const int *ligne_A = &A[i * m];

        // Étape 1 : Élagage rapide grâce au tri de la ligne
        // Si le premier élément de B est inférieur au minimum de la ligne de A,
        // ou si le dernier de la ligne de B dépasse le maximum de la ligne de A,
        // alors la sous-matrice ne peut pas se trouver ici !
        if (ligne_A[0] > cible || ligne_A[m - 1] < dernier_B0) {
            continue; // Rejet instantané en O(1)
        }

        // Étape 2 : Localiser la première colonne j où A[i][j] == cible par dichotomie
        int j = dicho_premiere_occurrence(ligne_A, max_j + 1, cible);

        // Si cible trouvée, tester les colonnes candidates
        while (j != -1 && j <= max_j && ligne_A[j] == cible) {

            // Vérification rapide des extrémités de chaque ligne du bloc
            int correspond = 1;
            for (int di = 0; di < n_prime; di++) {
                const int *ligne_Adi = &A[(i + di) * m];
                const int *ligne_Bdi = &B[di * m_prime];

                // Test rapide des deux extrémités de la ligne
                if (ligne_Adi[j] != ligne_Bdi[0] || 
                    ligne_Adi[j + m_prime - 1] != ligne_Bdi[m_prime - 1]) {
                    correspond = 0;
                    break;
                }

                // Test des éléments intérieurs
                for (int dj = 1; dj < m_prime - 1; dj++) {
                    if (ligne_Adi[j + dj] != ligne_Bdi[dj]) {
                        correspond = 0;
                        break;
                    }
                }
                if (!correspond) break;
            }

            if (correspond) {
                if (pos_i) *pos_i = i;
                if (pos_j) *pos_j = j;
                return 1; // Trouvé !
            }

            j++; // Tester la position suivante en cas de doublons successifs
        }
    }
    return 0; // Non trouvé
}
