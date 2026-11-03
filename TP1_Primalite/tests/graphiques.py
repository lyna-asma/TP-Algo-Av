"""Trace tous les graphiques requis du TP1 a partir des fichiers Excel (.xlsx) du dossier resultats/.

Usage :
    python tests/graphiques.py
"""
import glob                       # Permet de chercher tous les fichiers *.xlsx dans un dossier
import os                         # Permet de manipuler les chemins de fichiers et creer des dossiers
import xml.etree.ElementTree as ET  # Permet de lire le contenu XML interne d'un fichier Excel (.xlsx)
import zipfile                    # Permet d'ouvrir le fichier .xlsx (qui est une archive ZIP)
from collections import defaultdict  # Dictionnaire automatique pour grouper les temps par nombre de chiffres
import matplotlib.pyplot as plt   # Bibliotheque pour tracer et sauvegarder les graphiques

# Liste des 4 algorithmes de primalite demandes par la professeure
ALGOS = ["A1", "A2", "A3", "A4"]

# Chemin absolu du dossier 'tests/' ou se trouve ce script Python
DOSSIER = os.path.dirname(os.path.abspath(__file__))
# Chemin du sous-dossier 'tests/resultats/' qui contient les fichiers Excel (.xlsx)
RESULTATS = os.path.join(DOSSIER, "resultats")
# Chemin du sous-dossier 'tests/graphes/' ou seront enregistrees les images (.png)
GRAPHES = os.path.join(DOSSIER, "graphes")
# Cree le dossier 'tests/graphes/' s'il n'existe pas encore
os.makedirs(GRAPHES, exist_ok=True)

# Nettoie automatiquement les anciennes images .png avant de generer les nouvelles
for ancien_png in glob.glob(os.path.join(GRAPHES, "*.png")):
    os.remove(ancien_png)


def lire_xlsx(fichier):
    """Lit les lignes mesurees [N, chiffres, premier, T_A1, T_A2, T_A3, T_A4] d'un fichier .xlsx."""
    # Espace de noms (namespace) standard utilise par Microsoft Excel dans ses fichiers XML
    ns_xml = "{http://schemas.openxmlformats.org/spreadsheetml/2006/main}"
    # On ouvre le fichier .xlsx comme une archive ZIP en lecture
    with zipfile.ZipFile(fichier) as z:
        # On lit la feuille de calcul principale (sheet1.xml) et on la convertit en arbre XML
        racine = ET.fromstring(z.read("xl/worksheets/sheet1.xml"))
    # Liste qui va contenir toutes les lignes de mesures valides du tableau
    lignes = []
    # On parcourt chaque ligne (<row>) de la feuille Excel
    for row in racine.iter(ns_xml + "row"):
        # On extrait toutes les valeurs numeriques (<v>) presentes dans cette ligne et on les convertit en float
        vals = [float(v.text) for v in row.iter(ns_xml + "v")]
        # Une ligne de mesure complete contient exactement 7 nombres : [N, chiffres, premier, T_A1, T_A2, T_A3, T_A4]
        # Cela ignore automatiquement l'en-tete (0 nombre), les lignes "-" (3 nombres) et MIN/MAX/MOYENNE/SOMME (4 nombres)
        if len(vals) == 7:
            lignes.append(vals)
    # On renvoie la liste des lignes de mesures
    return lignes


# =====================================================================
# PARTIE 1 : Graphes pour chaque jeu de donnees (Random100, Random1000, Test-1, Test-2, Test-3, Test-TP1)
# =====================================================================
for fichier in sorted(glob.glob(os.path.join(RESULTATS, "*.xlsx"))):
    # Nom du fichier sans extension (ex: "Random100", "Test-1", "Test-TP1"...)
    nom = os.path.splitext(os.path.basename(fichier))[0]
    # On lit les mesures et on les trie par N croissant (l[0] = N) pour tracer de gauche a droite
    lignes = sorted(lire_xlsx(fichier))
    if not lignes:
        continue

    # Axe horizontal X : les nombres premiers N (colonne 0)
    valeurs_N = [l[0] for l in lignes]

    # --- 1.A : Graphe du temps T(N) en fonction de N (echelle log) ---
    plt.figure(figsize=(8, 5))
    for i, algo in enumerate(ALGOS):
        # Colonne 3 = T_A1, 4 = T_A2, 5 = T_A3, 6 = T_A4
        temps = [l[3 + i] for l in lignes]
        plt.plot(valeurs_N, temps, marker="o", markersize=3, label=algo)
    plt.xscale("log")
    plt.yscale("log")
    plt.xlabel("Nombre premier N")
    plt.ylabel("Temps d'execution T (en secondes)")
    plt.title(f"{nom} : Temps d'execution T(N) des 4 algorithmes")
    plt.grid(True, which="both", alpha=0.3)
    plt.legend()
    plt.tight_layout()
    plt.savefig(os.path.join(GRAPHES, f"{nom}_temps_vs_N.png"), dpi=150)
    plt.close()

    # --- 1.B : Graphe comparatif MIN, MOYENNE et MAX de chaque algorithme ---
    mins = [min(l[3 + i] for l in lignes) for i in range(4)]
    moys = [sum(l[3 + i] for l in lignes) / len(lignes) for i in range(4)]
    maxs = [max(l[3 + i] for l in lignes) for i in range(4)]

    plt.figure(figsize=(8, 5))
    plt.plot(ALGOS, mins, marker="o", label="Temps MIN")
    plt.plot(ALGOS, moys, marker="o", label="Temps MOYEN")
    plt.plot(ALGOS, maxs, marker="o", label="Temps MAX")
    plt.yscale("log")
    plt.xlabel("Algorithme")
    plt.ylabel("Temps (en secondes)")
    plt.title(f"{nom} : Comparaison Temps MIN, MOYEN et MAX par algorithme")
    plt.grid(True, which="both", alpha=0.3)
    plt.legend()
    plt.tight_layout()
    plt.savefig(os.path.join(GRAPHES, f"{nom}_min_moy_max.png"), dpi=150)
    plt.close()

    # --- 1.C : Graphe du temps moyen par nombre de chiffres (pour Test-2 et Test-3) ---
    if nom in ("Test-2", "Test-3"):
        par_chiffres = defaultdict(lambda: defaultdict(list))
        for l in lignes:
            nb_ch = int(l[1])  # Colonne 1 = nombre de chiffres
            for i, algo in enumerate(ALGOS):
                par_chiffres[algo][nb_ch].append(l[3 + i])
        plt.figure(figsize=(8, 5))
        for algo in ALGOS:
            tailles = sorted(par_chiffres[algo])
            moyennes = [sum(par_chiffres[algo][t]) / len(par_chiffres[algo][t]) for t in tailles]
            plt.plot(tailles, moyennes, marker="o", label=algo)
        plt.yscale("log")
        plt.xlabel("Nombre de chiffres de N")
        plt.ylabel("Temps moyen d'execution (en secondes)")
        plt.title(f"{nom} : Temps moyen en fonction du nombre de chiffres")
        plt.grid(True, which="both", alpha=0.3)
        plt.legend()
        plt.tight_layout()
        plt.savefig(os.path.join(GRAPHES, f"{nom}_par_chiffres.png"), dpi=150)
        plt.close()


# =====================================================================
# PARTIE 2 : Graphes specifiques demandes question par question dans l'enonce du TP1 (sur Test-TP1.xlsx)
# =====================================================================
fichier_tp1 = os.path.join(RESULTATS, "Test-TP1.xlsx")
if os.path.exists(fichier_tp1):
    lignes_tp1 = sorted(lire_xlsx(fichier_tp1))
    valeurs_N = [l[0] for l in lignes_tp1]
    t_A1 = [l[3] for l in lignes_tp1]

    # --- Question 3.e : 2 graphes pour A1 (Temps d'execution T(N) et Complexite au pire cas O(N)) ---
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))
    # Graphe gauche : Temps experimental T(N) de A1
    ax1.plot(valeurs_N, t_A1, marker="o", color="tab:blue", label="Temps mesure T(N) de A1")
    ax1.set_xlabel("Nombre premier N")
    ax1.set_ylabel("Temps d'execution T(N) (en secondes)")
    ax1.set_title("A1 : Variation du temps d'execution T(N)")
    ax1.grid(True, alpha=0.3)
    ax1.legend()
    # Graphe droite : Complexite theorique au pire cas O(N) (nombre de divisions N - 2)
    iterations_A1 = [n - 2 for n in valeurs_N]
    ax2.plot(valeurs_N, iterations_A1, marker="s", color="tab:purple", label="Complexite au pire cas O(N) ≈ N-2")
    ax2.set_xlabel("Nombre premier N")
    ax2.set_ylabel("Nombre d'operations (divisions)")
    ax2.set_title("A1 : Variation de la complexite au pire cas O(N)")
    ax2.grid(True, alpha=0.3)
    ax2.legend()
    plt.tight_layout()
    plt.savefig(os.path.join(GRAPHES, "TP1_Q3e_A1_temps_et_complexite.png"), dpi=150)
    plt.close()

    # --- Algorithme 2 (Q.2), Algorithme 3 (Q.2), Algorithme 4 (Q.4) : Comparaisons progressives (echelle lineaire) ---
    groupes = [
        (["A1", "A2"], "TP1_Comparaison_A1_A2.png", "Comparaison de A1 et A2 (echelle lineaire)"),
        (["A1", "A2", "A3"], "TP1_Comparaison_A1_A2_A3.png", "Comparaison de A1, A2 et A3 (echelle lineaire)"),
        (["A1", "A2", "A3", "A4"], "TP1_Comparaison_A1_A2_A3_A4.png", "Comparaison de A1, A2, A3 et A4 (echelle lineaire)"),
    ]
    for sous_liste, nom_img, titre in groupes:
        plt.figure(figsize=(8, 5))
        for i, algo in enumerate(sous_liste):
            temps = [l[3 + i] for l in lignes_tp1]
            plt.plot(valeurs_N, temps, marker="o", label=algo)
        plt.xlabel("Nombre premier N")
        plt.ylabel("Temps d'execution T (en secondes)")
        plt.title(titre)
        plt.grid(True, alpha=0.3)
        plt.legend()
        plt.tight_layout()
        plt.savefig(os.path.join(GRAPHES, nom_img), dpi=150)
        plt.close()

print("Tous les graphiques ont ete generes dans :", GRAPHES)
