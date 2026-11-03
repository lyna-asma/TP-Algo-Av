"""Trace les graphiques du TP1 a partir des fichiers Excel (.xlsx) du dossier resultats/.

Usage :
    python tests/graphiques.py
"""
import glob                       # Permet de chercher tous les fichiers *.xlsx dans un dossier
import os                         # Permet de manipuler les chemins de fichiers et creer des dossiers
import xml.etree.ElementTree as ET  # Permet de lire le contenu XML interne d'un fichier Excel (.xlsx)
import zipfile                    # Permet d'ouvrir le fichier .xlsx (qui est une archive ZIP)
import matplotlib.pyplot as plt   # Bibliotheque pour tracer et sauvegarder les graphiques

# Liste des 4 algorithmes de primalite a comparer sur chaque graphique
ALGOS = ["A1", "A2", "A3", "A4"]



# constantes de chemins de fichiers pour le script
# Chemin absolu du dossier 'tests/' ou se trouve ce script Python
DOSSIER = os.path.dirname(os.path.abspath(__file__))
# Chemin du sous-dossier 'tests/resultats/' qui contient les fichiers Excel (.xlsx)
RESULTATS = os.path.join(DOSSIER, "resultats")
# Chemin du sous-dossier 'tests/graphes/' ou seront enregistrees les images (.png)
GRAPHES = os.path.join(DOSSIER, "graphes")
# Cree le dossier 'tests/graphes/' s'il n'existe pas encore (exist_ok=True evite une erreur s'il existe deja)
os.makedirs(GRAPHES, exist_ok=True)

# Fonction pour lire les lignes de mesures d'un fichier Excel (.xlsx)
# lignes de mesures et pas de statistiques (min, max, moyenne) car on ne veut que les valeurs de N et les temps d'execution pour tracer les courbes
def lire_xlsx(fichier):
    """Lit les lignes mesurees d'un fichier .xlsx."""
    # Espace de noms (namespace) standard utilise par Microsoft Excel dans ses fichiers XML
    # Cela permet de retrouver les balises <row> et <v> dans le fichier XML
    # pourquoi xml , parce que le fichier .xlsx est en fait un fichier ZIP contenant des fichiers XML
    # comment les fichiers XML sont organisés dans le fichier .xlsx :
    # - xl/worksheets/sheet1.xml : contient les donnees de la feuille de calcul principale (les lignes de mesures)
    # - xl/sharedStrings.xml : contient les chaines de caracteres (texte) utilisees dans la feuille Excel
    # - xl/styles.xml : contient les styles de mise en forme (polices, couleurs, etc.) de la feuille Excel
    # et le fichier .xlsx contient d'autres fichiers XML pour les metadonnees, les images, etc.
    # les fonctions de lecture de ce script ne s'interessent qu'aux donnees numeriques de la feuille principale (sheet1.xml)
    # les fonctions min et max et moye sont ignorees car elles ne contiennent pas 7 nombres, donc elles ne sont pas ajoutees a la liste des lignes de mesures
    # lignes de mesures valides : [N, chiffres, premier, T_A1, T_A2, T_A3, T_A4] (7 nombres)
    # une mesure dans ce contexte disigne une ligne du tableau Excel qui contient le nombre premier N, le nombre de chiffres, un indicateur si N est premier ou non, et les temps d'execution des 4 algorithmes A1, A2, A3 et A4.

# ici le lien entre le fichier .xlsx et les lignes de mesures est fait par la lecture du fichier XML sheet1.xml, qui contient les balises <row> pour chaque ligne et <v> pour chaque valeur numerique. Le code extrait ces valeurs et les convertit en float pour les stocker dans une liste de lignes de mesures.
    # l espace de noms (namespace) est utilise pour identifier correctement les balises XML dans le fichier sheet1.xml, car elles sont precedees d'un prefixe qui indique qu'elles appartiennent a un schema XML specifique. Cela permet d'eviter les conflits de noms avec d'autres schemas XML qui pourraient etre utilises dans le meme fichier .xlsx.
    ns_xml = "{http://schemas.openxmlformats.org/spreadsheetml/2006/main}"
    # On ouvre le fichier .xlsx comme une archive ZIP en lecture
    # c est quoi archive ZIP : un fichier ZIP est un format de compression qui permet de regrouper plusieurs fichiers dans un seul fichier compressé. 
    # Le format .xlsx est en fait un fichier ZIP contenant plusieurs fichiers XML et autres ressources (images, styles, etc.) qui composent le classeur Excel. En ouvrant le fichier .xlsx comme une archive ZIP, on peut accéder aux fichiers internes (comme sheet1.xml) pour lire les données.
    with zipfile.ZipFile(fichier) as z:
        # On lit la feuille de calcul principale (sheet1.xml) et on la convertit en arbre XML
        # un arbre xml  est une structure de données qui représente un document XML sous forme d'objets hiérarchiques. Chaque élément XML est représenté par un nœud dans l'arbre, avec des relations parent-enfant entre les éléments. Cela permet de parcourir et de manipuler facilement le contenu du document XML.
        racine = ET.fromstring(z.read("xl/worksheets/sheet1.xml"))
    # Liste qui va contenir toutes les lignes de mesures valides du tableau
   # mesure valide veut dire ligne , ou bien row
    lignes = []
    # On parcourt chaque ligne (<row>) de la feuille Excel
    for row in racine.iter(ns_xml + "row"):
        # On extrait toutes les valeurs numeriques (<v>) presentes dans cette ligne et on les convertit en float
        vals = [float(v.text) for v in row.iter(ns_xml + "v")]
        # Une ligne de mesure complete contient exactement 7 nombres : [N, chiffres, premier, T_A1, T_A2, T_A3, T_A4]
        # Cela ignore automatiquement : l'en-tete (0 nombre), les lignes non mesurees "-" (3 nombres),
        # et les lignes de statistiques MIN / MAX / MOYENNE / SOMME en bas du tableau (4 nombres).
        if len(vals) == 7:
            # On ajoute la ligne de mesure a la liste des lignes valides
            # VALS = une seule Ligne 
            # Lignes = liste d elements de type vals
            lignes.append(vals)
    # On renvoie la liste a la fonction appelante pour qu'elle puisse les utiliser pour tracer les graphiques
    # lignes est de type list[list[float]], c est a dire une liste de listes de nombres flottants. Chaque sous-liste représente une ligne de mesures avec 7 valeurs : [N, chiffres, premier, T_A1, T_A2, T_A3, T_A4].
    return lignes


# Fonction principale qui trace les graphiques a partir des fichiers Excel (.xlsx) du dossier 'tests/resultats/'
# On parcourt tous les fichiers .xlsx du dossier 'tests/resultats/' par ordre alphabetique
for fichier in sorted(glob.glob(os.path.join(RESULTATS, "*.xlsx"))):
    # On recupere le nom du test sans le dossier ni l'extension .xlsx (ex: "Test-TP1" ou "Random100")
    nom = os.path.splitext(os.path.basename(fichier))[0]
    # On lit les mesures du fichier Excel et on les trie par N croissant (car l[0] = N)
    # Le tri est indispensable pour que la courbe relie les points de gauche a droite sans faire de zigzags
    lignes = sorted(lire_xlsx(fichier))

    # Axe horizontal X : on extrait la colonne 0 (le nombre premier N) de chaque ligne
    valeurs_N = [l[0] for l in lignes]

    # On cree une nouvelle figure de taille 8 x 5 pouces
    plt.figure(figsize=(8, 5))
    # Boucle sur les 4 algorithmes : i vaut 0 (A1), 1 (A2), 2 (A3), 3 (A4)
    # i est l'indice de l'algorithme dans la liste ALGOS, et algo est le nom de l'algorithme correspondant (A1, A2, A3 ou A4)
    # ils servent pour extraire les temps d'execution de chaque algorithme dans la colonne correspondante du tableau Excel (colonne 3 + i)
    # 3 est l'indice de la colonne T_A1 dans le tableau Excel, donc pour A2 on prend 4, pour A3 on prend 5, et pour A4 on prend 6
    for i, algo in enumerate(ALGOS):
        # Axe vertical Y : on extrait le temps de l'algorithme courant
        # Colonne 3 = T_A1, Colonne 4 = T_A2, Colonne 5 = T_A3, Colonne 6 = T_A4 (soit l[3 + i])
        # formule veut dire 
        temps = [l[3 + i] for l in lignes]
        # On trace la courbe (X = valeurs_N, Y = temps) avec des petits points ronds ("o") et sa legende
        # legende = nom de l'algorithme (A1, A2, A3 ou A4)
    # plt est une fonction de la bibliothèque matplotlib qui permet de tracer des graphiques en 2D. Elle prend en entrée les coordonnées X et Y des points à tracer, ainsi que des options de style pour personnaliser l'apparence du graphique.
    # Dans ce cas, elle trace les temps d'exécution des algorithmes en fonction des nombres premiers N, avec des marqueurs ronds ("o") de taille 3 et une légende indiquant le nom de l'algorithme.
        plt.plot(valeurs_N, temps, marker="o", markersize=3, label=algo)

    # Echelle logarithmique sur l'axe X (puissances de 10) pour bien repartir petits et grands nombres N
    plt.xscale("log")
    # Echelle logarithmique sur l'axe Y pour voir a la fois les temps en microsecondes et en secondes
    # les images sont en seco
    plt.yscale("log")
    # Titre de l'axe horizontal X
    plt.xlabel("Nombre premier N")
    # Titre de l'axe vertical Y
    plt.ylabel("Temps d'execution T (en secondes)")
    # Titre principal du graphique en haut de l'image
    plt.title(f"{nom} : Temps d'execution en fonction de N")
    # Affiche une grille legere (alpha=0.3) sur les graduations principales et secondaires (which="both")
    plt.grid(True, which="both", alpha=0.3)
    # Affiche l'encadre de legende (A1, A2, A3, A4)
    plt.legend()
    # Ajuste automatiquement les marges pour qu'aucun texte ne soit coupe
    plt.tight_layout()
    # Sauvegarde le graphique en image PNG (resolution 150 dpi) dans 'tests/graphes/<nom>.png'
    plt.savefig(os.path.join(GRAPHES, f"{nom}.png"), dpi=150)
    # Ferme la figure pour liberer la memoire avant de passer au fichier suivant
    plt.close()
    # Affiche un message de confirmation dans le terminal
    print(f"{nom}.png genere ({len(lignes)} valeurs mesurees)")
