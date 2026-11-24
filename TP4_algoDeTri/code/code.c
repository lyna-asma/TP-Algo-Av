#include <stdlib.h>
#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>

void permuter(int *a, int *b) {
    // *a est la valeur pointée par a
    int temp= *a;
    *a= *b;
    *b= temp;
}

void triGnome(int T[], int n) {
    int i = 0;
    while (i < n - 1) {
        if ( T[i] > T[i + 1]) {
        // si ils sont pas dans le bon ordre permuter, puis reculer (ou avancer si on est déjà au début)
        permuter(&T[i], &T[i + 1]);
      i = (i == 0) ? 1 : i - 1;
        // &T[i] est l'adresse de T[i], donc un pointeur vers T[i]
        } else {
            i++;
        }
    }
}

void triBulle(int T[], int n) {
    for (int i = n - 1; i > 0; i--) {
        for (int j = 1; j <= i; j++) {
            if (T[j - 1] > T[j]) {
                permuter(&T[j - 1], &T[j]);
            }
        }
    }
}

int partitionner(int T[], int gauche, int droite) {
    int pivot = T[droite];
    int i = gauche - 1;

    for (int j = gauche; j < droite; j++) {
        if (T[j] <= pivot) {
            i++;
            permuter(&T[i], &T[j]);
        }
    }

    permuter(&T[i + 1], &T[droite]);

    return i + 1;
}

void triRapide(int T[], int gauche, int droite) {
    if (gauche < droite) {
        int positionPivot = partitionner(T, gauche, droite);

        triRapide(T, gauche, positionPivot - 1);
        triRapide(T, positionPivot + 1, droite);
    }
}

int cle(int nombre, int position) {
    while (position > 0) {
        nombre /= 10;
        position--;
    }
    return nombre % 10;
}

void triDistribution(int T[], int n) {
    if (n <= 1) return;

    int max = T[0];

    for (int i = 1; i < n; i++) {
        if (T[i] > max) {
            max = T[i];
        }
    }

    int *sortie = malloc(n * sizeof(int));
    if (sortie == NULL) return;

    int position = 0;

    do {
        int count[10] = {0};

        for (int i = 0; i < n; i++) {
            count[cle(T[i], position)]++;
        }

        for (int i = 1; i < 10; i++) {
            count[i] += count[i - 1];
        }

        for (int i = n - 1; i >= 0; i--) {
            int chiffre = cle(T[i], position);
            sortie[count[chiffre] - 1] = T[i];
            count[chiffre]--;
        }

        for (int i = 0; i < n; i++) {
            T[i] = sortie[i];
        }

        if (max == 0 || max / (int)1 < 0) break;

        position++;

        // Arrêter après le dernier chiffre significatif.
        int puissance = 1;
        for (int j = 0; j < position; j++) {
            puissance *= 10;
        }

        if (puissance > max) break;

    } while (1);

    free(sortie);
}

void entasser(int T[], int n, int i) {
    int max = i;
    int gauche = 2 * i + 1;
    int droite = 2 * i + 2;

    if (gauche < n && T[gauche] > T[max]) {
        max = gauche;
    }

    if (droite < n && T[droite] > T[max]) {
        max = droite;
    }

    if (max != i) {
        permuter(&T[i], &T[max]);
        entasser(T, n, max);
    }
}

void triTas(int T[], int n) {

    // Construction du tas maximum
    for (int i = n / 2 - 1; i >= 0; i--) {
        entasser(T, n, i);
    }

    // Extraction successive du maximum
    for (int i = n - 1; i > 0; i--) {
        permuter(&T[0], &T[i]);

        entasser(T, i, 0);
    }
}

void afficherTableau(int T[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", T[i]);
    }
    printf("\n");
}


    int main() {

    int T[] = {7, 3, 9, 2, 5, 1, 8, 4};
    int n = sizeof(T) / sizeof(T[0]);

    int A[n], B[n], C[n], D[n], E[n];

    // Copier le tableau initial
    for (int i = 0; i < n; i++) {
        A[i] = T[i];
        B[i] = T[i];
        C[i] = T[i];
        D[i] = T[i];
        E[i] = T[i];
    }

    printf("Tableau initial : ");
    afficherTableau(T, n);

    triBulle(A, n);
    printf("Tri Bulle       : ");
    afficherTableau(A, n);

    triGnome(B, n);
    printf("Tri Gnome       : ");
    afficherTableau(B, n);

    triRapide(C, 0, n - 1);
    printf("Tri Rapide      : ");
    afficherTableau(C, n);

    triDistribution(D, n);
    printf("Tri Distribution: ");
    afficherTableau(D, n);

    triTas(E, n);
    printf("Tri Tas         : ");
    afficherTableau(E, n);

    return 0;
}

