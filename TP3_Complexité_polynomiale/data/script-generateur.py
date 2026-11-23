"""
TP3 - Complexite polynomiale : GENERATION DES DONNEES (matrices)

Prerequis : pip install numpy

Utilisation (depuis le dossier data/ du TP3) :
    python script-generateur.py               -> tout generer (Exercice 1 et 2, n jusqu'a 8000)
    python script-generateur.py --max-n 1000  -> seulement les tailles n <= 1000 (pour tester vite)
    python script-generateur.py --exercice 1  -> seulement l'exercice 1  (ou --exercice 2)

FORMAT DES FICHIERS (fichiers TEXTE .txt) :
    - 1re ligne : la taille n de la matrice carree (n lignes et n colonnes)
    - puis n lignes, chacune contenant n entiers separes par un espace
    Lecture en C :
        FILE *f = fopen("A_100.txt", "r");
        int n;  fscanf(f, "%d", &n);
        int *M = malloc((size_t)n * n * sizeof(int));
        for (size_t k = 0; k < (size_t)n * n; k++) fscanf(f, "%d", &M[k]);   // M[i*n + j] = (i, j)
        fclose(f);

EXERCICE 1 (produit de matrices C = A x B, matrices carrees n x n)
    A_<n>.txt, B_<n>.txt : GRANDS entiers aleatoires entre 1 et 1 000 000.
    ATTENTION (debordement) : chaque produit A[i][k]*B[k][j] peut atteindre 10^12 et la somme
    de n produits 8.10^15 (n = 8000) : cela depasse la taille d'un int (2.10^9). Il faut donc
    declarer la matrice resultat C, ou au moins la variable qui cumule la somme, en
    "long long" (64 bits, jusqu'a 9.10^18). Les elements de A et B, eux, tiennent dans un int.

EXERCICE 2 (recherche d'une sous-matrice B dans une matrice A)
    Matrices carrees. Pour chaque n on genere A (n x n) puis 5 sous-matrices B (k x k) :
        n <= 1000 : k dans (10, 20, 30, 40, 50)
        n >= 2000 : k dans (100, 200, 300, 400, 500)
    sousMat1 -> matrices NON triees :  A_nontriee_<n>.txt et B_nontriee_<n>_<k>.txt
    sousMat2 -> matrices TRIEES (chaque ligne en ordre croissant) :
                A_triee_<n>.txt     et B_triee_<n>_<k>.txt
    Valeurs : grands entiers aleatoires entre 0 et 10^9 (tiennent dans un int).
    B est TOUJOURS un bloc de A, pris tout en bas a droite (derniere position possible) :
    la recherche naive doit donc parcourir presque toute la matrice (cas defavorable),
    et on est sur que B existe. La position exacte est ecrite dans ex2_positions.csv
    (i0, j0, numerotation a partir de 0) : la fonction de recherche doit retrouver (i0, j0).
    Dans la version triee, B est pris dans la matrice A TRIEE, donc ses lignes sont triees aussi.

La graine (seed) est fixe : si on relance le script, on obtient exactement les memes donnees.
"""

import argparse
import os
import shutil
import sys
import time

try:
    import numpy as np
except ImportError:
    sys.exit("numpy est necessaire :  pip install numpy")

# Tailles n demandees par l'enonce / le tableau (cf. photos du tableau)
TAILLES = [100, 200, 400, 600, 800, 1000, 2000, 4000, 6000, 8000]

SEUIL = 2000                                   # n < SEUIL : petites sous-matrices ; sinon grandes
SOUS_TAILLES_PETITES = [10, 20, 30, 40, 50]
SOUS_TAILLES_GRANDES = [100, 200, 300, 400, 500]

VALEUR_MAX_EX1 = 1_000_000          # Ex1 : entiers entre 1 et 1 000 000 (cf. long long pour le produit)
VALEUR_MAX_EX2 = 1_000_000_000      # Ex2 : entiers entre 0 et 10^9 - 1

SEED = 42
DOSSIER = os.path.dirname(os.path.abspath(__file__))   # les fichiers sont ecrits a cote du script


def sauvegarder(nom, matrice):
    """Ecrit la matrice en TEXTE : 1re ligne = n, puis une ligne de la matrice par ligne du fichier."""
    with open(os.path.join(DOSSIER, nom), "w") as f:
        f.write(f"{matrice.shape[0]}\n")
        for ligne in matrice:
            f.write(" ".join(map(str, ligne.tolist())))
            f.write("\n")


def sous_tailles(n):
    return SOUS_TAILLES_PETITES if n < SEUIL else SOUS_TAILLES_GRANDES


def octets_estimes(tailles, ex1, ex2):
    """Estimation de l'espace disque necessaire (en octets) : ~7 caracteres par entier en Ex1,
    ~11 en Ex2 (nombres plus grands)."""
    total = 0
    for n in tailles:
        if ex1:
            total += 2 * n * n * 7                          # A et B
        if ex2:
            total += 2 * n * n * 11                         # A non triee + A triee
            total += 2 * sum(k * k * 11 for k in sous_tailles(n))   # les 5 B, en 2 versions
    return total


def exercice1(rng, n):
    A = rng.integers(1, VALEUR_MAX_EX1 + 1, size=(n, n), dtype=np.int32)    # 1 .. 1 000 000
    B = rng.integers(1, VALEUR_MAX_EX1 + 1, size=(n, n), dtype=np.int32)
    sauvegarder(f"A_{n}.txt", A)
    sauvegarder(f"B_{n}.txt", B)
    print(f"[Ex1] n = {n:>5} : A_{n}.txt et B_{n}.txt crees")


def exercice2(rng, n, csv):
    # Matrice A non triee : grands entiers aleatoires (tres peu de doublons)
    A = rng.integers(0, VALEUR_MAX_EX2, size=(n, n), dtype=np.int32)
    # Matrice A triee : chaque ligne est triee par ordre croissant
    A_triee = np.sort(A, axis=1)

    sauvegarder(f"A_nontriee_{n}.txt", A)
    sauvegarder(f"A_triee_{n}.txt", A_triee)

    for k in sous_tailles(n):
        i0 = j0 = n - k                                      # derniere position possible (bas-droite)
        B = A[i0:i0 + k, j0:j0 + k]                          # bloc k x k de A
        B_triee = A_triee[i0:i0 + k, j0:j0 + k]              # meme bloc dans la matrice triee

        # verifications : B est bien un bloc de A, et ses lignes sont triees dans la version triee
        assert np.array_equal(B, A[i0:i0 + k, j0:j0 + k])
        assert np.all(np.diff(B_triee, axis=1) >= 0)

        sauvegarder(f"B_nontriee_{n}_{k}.txt", B)
        sauvegarder(f"B_triee_{n}_{k}.txt", B_triee)
        csv.write(f"{n},{k},{i0},{j0}\n")
    print(f"[Ex2] n = {n:>5} : A (non triee / triee) + 5 sous-matrices B crees")


def main():
    p = argparse.ArgumentParser(description="Generateur de donnees TP3 (matrices)")
    p.add_argument("--max-n", type=int, default=max(TAILLES), help="plus grande taille n a generer")
    p.add_argument("--exercice", type=int, choices=[1, 2], help="ne generer qu'un seul exercice")
    p.add_argument("--texte", action="store_true", help="(ignore : les fichiers sont toujours en texte)")
    args = p.parse_args()

    tailles = [n for n in TAILLES if n <= args.max_n]
    ex1 = args.exercice in (None, 1)
    ex2 = args.exercice in (None, 2)

    besoin = octets_estimes(tailles, ex1, ex2)
    libre = shutil.disk_usage(DOSSIER).free
    print(f"Espace disque necessaire : {besoin / 1e6:,.0f} Mo (libre : {libre / 1e6:,.0f} Mo)")
    if besoin > libre * 0.95:
        sys.exit("Pas assez d'espace disque : utiliser --max-n pour generer moins de tailles.")

    rng = np.random.default_rng(SEED)
    print(f"Generation pour n dans {tailles}\n")

    debut = time.time()
    if ex2:
        csv = open(os.path.join(DOSSIER, "ex2_positions.csv"), "w")
        csv.write("n,k,i0,j0\n")
    for n in tailles:
        if ex1:
            exercice1(rng, n)
        if ex2:
            exercice2(rng, n, csv)
    if ex2:
        csv.close()

    print(f"\nGeneration terminee avec succes ! ({time.time() - debut:.0f} secondes)")


if __name__ == "__main__":
    main()