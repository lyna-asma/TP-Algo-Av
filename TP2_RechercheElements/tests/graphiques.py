"""
TP2 - GRAPHES a partir des CSV produits par mesures.c

Prerequis : pip install matplotlib
Utilisation (depuis le dossier tests/) :  python graphiques.py

Cree le dossier graphes/ avec :
  graphe_pire_cas.png      : T(N) au pire cas des 3 fonctions de recherche (Partie A)
  graphe_pire_cas_dicho.png: zoom sur la dichotomie (sinon sa courbe est ecrasee par les 2 autres)
  graphe_comparaisons_B.png: nombre de comparaisons MaxEtMinA vs MaxEtMinB (Partie B)
  graphe_temps_B.png       : temps MaxEtMinA vs MaxEtMinB (Partie B)
"""
import csv
import os
import sys

try:
    import matplotlib
    matplotlib.use("Agg")           # pas besoin d'ecran : on enregistre directement en PNG
    import matplotlib.pyplot as plt
except ImportError:
    sys.exit("matplotlib est necessaire :  pip install matplotlib")

DOSSIER = os.path.dirname(os.path.abspath(__file__))
SORTIE = os.path.join(DOSSIER, "graphes")


def lire(nom):
    """Lit un CSV et renvoie un dictionnaire {colonne: [valeurs en float]}."""
    chemin = os.path.join(DOSSIER, nom)
    if not os.path.exists(chemin):
        sys.exit(f"{nom} introuvable : lancer d'abord mesures (gcc -O0 mesures.c -o mesures)")
    with open(chemin, newline="") as f:
        lignes = list(csv.DictReader(f))
    return {col: [float(l[col]) for l in lignes] for col in lignes[0]}


def enregistrer(nom):
    plt.tight_layout()
    plt.savefig(os.path.join(SORTIE, nom), dpi=150)
    plt.close()
    print("cree :", os.path.join("graphes", nom))


os.makedirs(SORTIE, exist_ok=True)

# ---------------- Partie A : pire cas ----------------
A = lire("resultats_A.csv")
n = A["n"]

plt.figure(figsize=(8, 5))
plt.plot(n, A["nontrie_pire_us"], "o-", label="Recherche tableau non trie")
plt.plot(n, A["trie_pire_us"], "s-", label="Recherche sequentielle (trie)")
plt.plot(n, A["dicho_pire_us"], "^-", label="Recherche dichotomique")
plt.xlabel("N (taille du tableau)")
plt.ylabel("T(N) au pire cas (microsecondes)")
plt.title("Partie A : temps d'execution au pire cas")
plt.grid(True, alpha=0.3)
plt.legend()
enregistrer("graphe_pire_cas.png")

plt.figure(figsize=(8, 5))
plt.plot(n, A["dicho_pire_us"], "^-", color="tab:green", label="Recherche dichotomique")
plt.xlabel("N (taille du tableau)")
plt.ylabel("T(N) au pire cas (microsecondes)")
plt.title("Dichotomie au pire cas : croissance logarithmique")
plt.grid(True, alpha=0.3)
plt.legend()
enregistrer("graphe_pire_cas_dicho.png")

# ---------------- Partie B ----------------
B = lire("resultats_B.csv")
nb = B["n"]

plt.figure(figsize=(8, 5))
plt.plot(nb, B["A_comparaisons"], "o-", label="MaxEtMinA : 2(n-1)")
plt.plot(nb, B["B_comparaisons"], "s-", label="MaxEtMinB : ceil(3n/2)-2")
plt.xlabel("N (taille du tableau)")
plt.ylabel("Nombre de comparaisons")
plt.title("Partie B : nombre de comparaisons")
plt.grid(True, alpha=0.3)
plt.legend()
enregistrer("graphe_comparaisons_B.png")

plt.figure(figsize=(8, 5))
plt.plot(nb, B["A_temps_ms"], "o-", label="MaxEtMinA")
plt.plot(nb, B["B_temps_ms"], "s-", label="MaxEtMinB")
plt.xlabel("N (taille du tableau)")
plt.ylabel("Temps d'execution (ms)")
plt.title("Partie B : temps d'execution")
plt.grid(True, alpha=0.3)
plt.legend()
enregistrer("graphe_temps_B.png")
