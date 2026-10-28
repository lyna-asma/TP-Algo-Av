"""Trace les graphiques du TP1 a partir des CSV du dossier resultats/.

Usage (depuis le dossier TP1_Primalite) :
    python graphiques.py
Cree le dossier graphes/ avec une image PNG par graphique.
"""
import csv
import glob
import os
from collections import defaultdict

import matplotlib.pyplot as plt

ALGOS = ["A1", "A2", "A3", "A4", "A5", "A6"]
os.makedirs("graphes", exist_ok=True)


def lire(fichier):
    """Lit un CSV (separateur ;) et renvoie une liste de dictionnaires numeriques."""
    lignes = []
    with open(fichier, newline="", encoding="utf-8-sig") as f:
        for row in csv.DictReader(f, delimiter=";"):
            try:
                lignes.append({k: float(v) for k, v in row.items()})
            except (ValueError, TypeError):
                pass  # ligne incomplete (programme interrompu) : on l'ignore
    return lignes


for fichier in sorted(glob.glob("resultats/*.csv")):
    nom = os.path.splitext(os.path.basename(fichier))[0]
    data = lire(fichier)
    if not data:
        print(f"{nom} : vide, ignore")
        continue

    # --- Graphique 1 : temps de chaque algo en fonction de N (echelles log) ---
    plt.figure(figsize=(8, 5))
    for a in ALGOS:
        pts = sorted((d["N"], d["T_" + a]) for d in data if d["T_" + a] >= 0)  # -1 = non mesure
        if pts:
            plt.plot(*zip(*pts), marker="o", markersize=3, label=a)
    plt.xscale("log")
    plt.yscale("log")
    plt.xlabel("N")
    plt.ylabel("Temps moyen d'un appel (s)")
    plt.title(f"{nom} : temps en fonction de N")
    plt.grid(True, which="both", alpha=0.3)
    plt.legend()
    plt.tight_layout()
    plt.savefig(f"graphes/{nom}_temps_vs_N.png", dpi=150)
    plt.close()

    # --- Graphique 2 : temps moyen par nombre de chiffres ---
    par_chiffres = defaultdict(lambda: defaultdict(list))
    for d in data:
        for a in ALGOS:
            if d["T_" + a] >= 0:
                par_chiffres[a][int(d["chiffres"])].append(d["T_" + a])
    plt.figure(figsize=(8, 5))
    for a in ALGOS:
        if par_chiffres[a]:
            xs = sorted(par_chiffres[a])
            plt.plot(xs, [sum(par_chiffres[a][x]) / len(par_chiffres[a][x]) for x in xs],
                     marker="o", label=a)
    plt.yscale("log")
    plt.xlabel("Nombre de chiffres")
    plt.ylabel("Temps moyen (s)")
    plt.title(f"{nom} : temps moyen par nombre de chiffres")
    plt.grid(True, which="both", alpha=0.3)
    plt.legend()
    plt.tight_layout()
    plt.savefig(f"graphes/{nom}_par_chiffres.png", dpi=150)
    plt.close()

    # --- Moyenne de chaque algo sur tout le fichier ---
    print(f"\n{nom} ({len(data)} lignes) - temps moyen :")
    for a in ALGOS:
        v = [d["T_" + a] for d in data if d["T_" + a] >= 0]
        print(f"  {a} : {sum(v) / len(v):.9f} s" if v else f"  {a} : non mesure")

print("\nGraphiques enregistres dans le dossier graphes/")

