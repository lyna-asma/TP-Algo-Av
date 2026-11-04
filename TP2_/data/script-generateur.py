import os
import random

# Tailles N demandees dans l'enonce du TP2 :
# - Partie A (Q3) : de 100 000 a 1 800 000
# - Partie B      : de 100 000 a 800 000 (les 5 premieres tailles)
TAILLES = [
    100_000,
    200_000,
    400_000,
    600_000,
    800_000,
    1_000_000,
    1_200_000,
    1_400_000,
    1_600_000,
    1_800_000,
]

# Dossier de sortie (toujours dans TP2_/data/, peu importe d'ou on lance le script)
DOSSIER_DATA = os.path.dirname(os.path.abspath(__file__))


def generer_tableau_aleatoire_distinct(n):
    """
    Tire aleatoirement n entiers TOUS DISTINCTS (sans doublons) dans [1, 10*n].
    Note : contrairement a random.randint(), random.sample() fait un tirage
    SANS REMISE, ce qui garantit 0 repetition.
    """
    return random.sample(range(1, 10 * n + 1), n)


def sauvegarder(nom_fichier, donnees):
    chemin = os.path.join(DOSSIER_DATA, nom_fichier)
    with open(chemin, "w", encoding="utf-8") as f:
        f.write("\n".join(map(str, donnees)) + "\n")
    print(f"{nom_fichier:<24} : {len(donnees):>9} elements | 5 premiers -> {donnees[:5]}")


if __name__ == "__main__":
    random.seed(42)  # Graine fixe pour rendre les tests reproductibles

    print("=== Generation des jeux de donnees (Non Tries & Tries, sans doublons) ===")
    for n in TAILLES:
        # 1. Jeu NON TRIE (tous les nombres sont distincts) -> pour Partie A.1 et Partie B
        data_non_trie = generer_tableau_aleatoire_distinct(n)
        sauvegarder(f"non_trie_{n}.txt", data_non_trie)

        # 2. Jeu TRIE (les memes nombres distincts, tries par ordre croissant) -> pour Partie A.2
        data_trie = sorted(data_non_trie)
        sauvegarder(f"trie_{n}.txt", data_trie)

    print("\nGeneration terminee avec succes !")
