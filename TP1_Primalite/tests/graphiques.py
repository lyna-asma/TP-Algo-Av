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
# Sous-dossiers organises par categorie de graphe
DOSSIER_VS_N = os.path.join(GRAPHES, "comparaison-temps-vs-N(valeur-de-N)")
DOSSIER_MIN_MOY_MAX = os.path.join(GRAPHES, "min-moy-max")
DOSSIER_PAR_CHIFFRES = os.path.join(GRAPHES, "comparaison-par-chiffres(taille-de-N)")
DOSSIER_GRADUELLE = os.path.join(GRAPHES, "comparaison-graduelle")

for d in (GRAPHES, DOSSIER_VS_N, DOSSIER_MIN_MOY_MAX, DOSSIER_PAR_CHIFFRES, DOSSIER_GRADUELLE):
    os.makedirs(d, exist_ok=True)


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
        # Cas 1 : les 4 algos sont mesures -> 7 nombres [N, chiffres, premier, T_A1, T_A2, T_A3, T_A4]
        if len(vals) == 7:
            lignes.append(vals)
        # Cas 2 (grands nombres de 10 a 12 chiffres) : A1 et A2 sont "-" (non mesures), seuls A3 et A4 sont mesures -> 5 nombres
        elif len(vals) == 5:
            lignes.append([vals[0], vals[1], vals[2], -1.0, -1.0, vals[3], vals[4]])
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

    # --- 1.A : Graphe du temps T(N) en fonction de N (echelle log) ---
    plt.figure(figsize=(8, 5))
    for i, algo in enumerate(ALGOS):
        # On filtre les valeurs >= 0 (pour ignorer les -1 des algos A1/A2 au-dela de 9 chiffres)
        pts = [(l[0], l[3 + i]) for l in lignes if l[3 + i] >= 0]
        if pts:
            xs, ys = zip(*pts)
            plt.plot(xs, ys, marker="o", markersize=3, label=algo)
    plt.xscale("log")
    plt.yscale("log")
    plt.xlabel("Nombre premier N")
    plt.ylabel("Temps d'execution T (en secondes)")
    plt.title(f"{nom} : Temps d'execution T(N) des 4 algorithmes")
    plt.grid(True, which="both", alpha=0.3)
    plt.legend()
    plt.tight_layout()
    plt.savefig(os.path.join(DOSSIER_VS_N, f"{nom}_temps_vs_N.png"), dpi=150)
    plt.close()

    # --- 1.B : Graphe comparatif MIN, MOYENNE et MAX de chaque algorithme ---
    mins, moys, maxs = [], [], []
    for i in range(4):
        vals_algo = [l[3 + i] for l in lignes if l[3 + i] >= 0]
        mins.append(min(vals_algo))
        moys.append(sum(vals_algo) / len(vals_algo))
        maxs.append(max(vals_algo))

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
    plt.savefig(os.path.join(DOSSIER_MIN_MOY_MAX, f"{nom}_min_moy_max.png"), dpi=150)
    plt.close()

    # --- 1.C : Graphe du temps moyen par nombre de chiffres (pour Test-2 et Test-3) ---
    if nom in ("Test-2", "Test-3"):
        par_chiffres = defaultdict(lambda: defaultdict(list))
        for l in lignes:
            nb_ch = int(l[1])  # Colonne 1 = nombre de chiffres
            for i, algo in enumerate(ALGOS):
                if l[3 + i] >= 0:
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
        plt.savefig(os.path.join(DOSSIER_PAR_CHIFFRES, f"{nom}_par_chiffres.png"), dpi=150)
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

    # --- Algorithme 2 (Q.2), Algorithme 3 (Q.2), Algorithme 4 (Q.4) : Comparaisons progressives ---
    groupes = [
        (["A1", "A2"], "TP1_Comparaison_A1_A2.png", "Comparaison de A1 et A2", False),
        (["A1", "A2", "A3"], "TP1_Comparaison_A1_A2_A3.png", "Comparaison de A1, A2 et A3 (echelle log)", True),
        (["A1", "A2", "A3", "A4"], "TP1_Comparaison_A1_A2_A3_A4.png", "Comparaison de A1, A2, A3 et A4 (echelle log)", True),
    ]
    for sous_liste, nom_img, titre, use_log in groupes:
        plt.figure(figsize=(8, 5))
        for i, algo in enumerate(sous_liste):
            temps = [l[3 + i] for l in lignes_tp1]
            plt.plot(valeurs_N, temps, marker="o", label=algo)
        if use_log:
            plt.xscale("log")
            plt.yscale("log")
        plt.xlabel("Nombre premier N")
        plt.ylabel("Temps d'execution T (en secondes)")
        plt.title(titre)
        plt.grid(True, which="both", alpha=0.3)
        plt.legend()
        plt.tight_layout()
        plt.savefig(os.path.join(DOSSIER_GRADUELLE, nom_img), dpi=150)
        plt.close()

print("Tous les graphiques ont ete generes dans :", GRAPHES)
