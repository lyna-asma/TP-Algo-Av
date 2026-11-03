# TP1 – Résumé Complet pour la Validation (Données, Code, Tests, Excel et Graphes)

> **Lis ce document de haut en bas : il t'explique 100 % du TP en français simple, étape par étape, avec les réponses exactes à donner à la professeure.**

---

## 1. Comprendre tout le TP en 2 minutes (La vue d'ensemble)

Imagine que le TP est une chaîne en **3 étapes** :

1. **Étape 1 : Les Données (`data/`)**
   - Un script Python ([`script-data.py`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/data/script-data.py)) a généré des fichiers `.txt` remplis uniquement de **nombres premiers** (ex : `103`, `1000003`, `2048000011`...).
   - **Pourquoi uniquement des nombres premiers ?** Parce que pour vérifier qu'un nombre est premier, un algorithme cherche un diviseur. S'il n'en trouve aucun (cas d'un nombre premier), il est obligé de faire **tous les tours de boucle jusqu'à la fin**. C'est ce qu'on appelle tester le **pire cas**.

2. **Étape 2 : Le Code C (`code/tp1.c` et `tp1.exe`)**
   - Le programme C prend un fichier `.txt` de nombres premiers, et pour chaque nombre $N$, il chronomètre les **4 algorithmes** (`A1`, `A2`, `A3`, `A4`) en secondes :
     - **`A1`** : teste tous les diviseurs $i$ de $2$ jusqu'à $N-1$ (1 par 1) $\rightarrow$ fait **$N$ tours**.
     - **`A2`** : teste tous les diviseurs $i$ de $2$ jusqu'à $N/2$ (1 par 1) $\rightarrow$ fait **$N/2$ tours** (**2 fois plus rapide que `A1`**).
     - **`A3`** : élimine les nombres pairs, puis teste seulement les diviseurs impairs $3, 5, 7\dots$ jusqu'à $N-2$ (de 2 en 2) $\rightarrow$ fait **$N/2$ tours** (**2 fois plus rapide que `A1`, même vitesse que `A2`**).
     - **`A4`** : élimine les nombres pairs, puis teste seulement les diviseurs impairs $3, 5, 7\dots$ jusqu'à $N/2$ (de 2 en 2) $\rightarrow$ fait **$N/4$ tours** (**4 fois plus rapide que `A1`, c'est le meilleur !**).

3. **Étape 3 : Ta partie — Les Tests, les fichiers Excel et les Graphes (`tests/`)**
   - On a exécuté `tp1.exe` sur **les 6 fichiers de données** et rangé les résultats dans **`tests/resultats/*.xlsx`**.
   - En bas de chaque fichier Excel `.xlsx`, il y a 4 lignes calculées par formules Excel pour chaque algorithme :
     - **`MIN`** : le temps le plus court (sur le plus petit nombre du fichier),
     - **`MAX`** : le temps le plus long (sur le plus grand nombre du fichier),
     - **`MOYENNE`** : la moyenne des temps sur tous les nombres du fichier,
     - **`SOMME`** : le temps total cumulé sur tout le fichier.
   - Enfin, ton script [`tests/graphiques.py`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/graphiques.py) lit ces 6 fichiers Excel `.xlsx` et trace automatiquement tous les graphes dans **`tests/graphes/*.png`**.

---

## 2. Quels sont les 6 cas (jeux de données) testés et pourquoi ?

Si la prof te demande : *« Quelles données avez-vous utilisées pour vos tests ? »*, voici la réponse exacte :

### A. L'échantillon de 12 valeurs de l'énoncé du TP1 (`Test-TP1.xlsx`)
- Ce sont les **12 nombres premiers donnés dans le tableau du sujet TP1** :
  `1000003`, `2000003`, `4000037`, `8000009`, `16000057`, `32000011`, `64000031`, `128000003`, `256000001`, `512000009`, `1024000009`, `2048000011`.
- **Particularité** : chaque nombre est le **double** du précédent ($N_{k+1} \approx 2 \times N_k$).
- **Ce qu'on observe** : à chaque fois que $N$ double, le temps mesuré $T(N)$ **double aussi** ! Cela prouve expérimentalement que la complexité des 4 algorithmes est **linéaire en $O(N)$**.

### B. Les 5 jeux de données du TP0 (`Random100`, `Random1000`, `Test-1`, `Test-2`, `Test-3`)
Le but du TP0 est de comparer des **tirages aléatoires non contrôlés** avec des **tirages contrôlés par taille (nombre de chiffres)** :
1. **`Random100.xlsx`** : 100 nombres premiers tirés au hasard ($\le 10^6$).
2. **`Random1000.xlsx`** : 1000 nombres premiers tirés au hasard ($\le 10^6$).
3. **`Test-1.xlsx`** : 100 nombres premiers tirés au hasard dans l'intervalle $[1\,000, 1\,000\,000]$.
   - *Remarque sur ces 3 premiers jeux* : comme il y a beaucoup plus de nombres premiers entre $100\,000$ et $1\,000\,000$ (6 chiffres) qu'entre $1$ et $10\,000$, un tirage au hasard sélectionne presque uniquement des nombres à 6 chiffres. Sur leurs graphes, tous les points sont tassés à droite !
4. **`Test-2.xlsx`** : échantillon **contrôlé** contenant exactement **10 nombres premiers pour chaque taille de 3 à 12 chiffres** (100 nombres au total ; mesurés de 3 à 9 chiffres car au-delà de $2{,}1 \times 10^9$, un algorithme en $O(N)$ mettrait des heures).
5. **`Test-3.xlsx`** : échantillon **structuré** de 40 nombres (10 à 3 chiffres, 10 à 6 chiffres, 10 à 9 chiffres, 10 à 12 chiffres).
   - *Remarque sur `Test-2` et `Test-3`* : grâce au contrôle par nombre de chiffres, les points sont parfaitement espacés sur le graphe. On voit que chaque fois qu'on ajoute **1 chiffre** ($N \times 10$), le temps est **multiplié par 10**.

---

## 3. Tous les tableaux de résultats (`MIN`, `MAX`, `MOYENNE`, `SOMME`)

### Tableau 1 : Les 12 valeurs de l'énoncé du TP1 ([`Test-TP1.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Test-TP1.xlsx))

| $N$ | Chiffres | Premier | $T_{A1}$ (s) | $T_{A2}$ (s) | $T_{A3}$ (s) | $T_{A4}$ (s) |
|---:|:---:|:---:|---:|---:|---:|---:|
| `1 000 003` | 7 | 1 | 0,003466667 | 0,001388889 | 0,001562500 | 0,000819672 |
| `2 000 003` | 7 | 1 | 0,006375000 | 0,003250000 | 0,003187500 | 0,001562500 |
| `4 000 037` | 7 | 1 | 0,012750000 | 0,012750000 | 0,006250000 | 0,003187500 |
| `8 000 009` | 7 | 1 | 0,025000000 | 0,012500000 | 0,012750000 | 0,006375000 |
| `16 000 057` | 8 | 1 | 0,062000000 | 0,024333333 | 0,025500000 | 0,013000000 |
| `32 000 011` | 8 | 1 | 0,104000000 | 0,051000000 | 0,052000000 | 0,025000000 |
| `64 000 031` | 8 | 1 | 0,206000000 | 0,102000000 | 0,101000000 | 0,047000000 |
| `128 000 003` | 9 | 1 | 0,412000000 | 0,208000000 | 0,208000000 | 0,113000000 |
| `256 000 001` | 9 | 1 | 0,821000000 | 0,415000000 | 0,414000000 | 0,217000000 |
| `512 000 009` | 9 | 1 | 1,654000000 | 0,821000000 | 0,818000000 | 0,409000000 |
| `1 024 000 009` | 10 | 1 | 3,270000000 | 1,654000000 | 1,650000000 | 0,799000000 |
| `2 048 000 011` | 10 | 1 | 6,619000000 | 3,305000000 | 3,281000000 | 1,641000000 |
| **MIN** | — | — | **0,003466667** | **0,001388889** | **0,001562500** | **0,000819672** |
| **MAX** | — | — | **6,619000000** | **3,305000000** | **3,281000000** | **1,641000000** |
| **MOYENNE** | — | — | **1,099632639** | **0,550851852** | **0,547770833** | **0,272995389** |
| **SOMME** | — | — | **13,195591667** | **6,610222222** | **6,573250000** | **3,275944672** |

### Tableau 2 : Résumé `MIN`, `MAX`, `MOYENNE`, `SOMME` des 5 jeux du TP0

| Fichier Excel | Statistique | `T_A1` (s) | `T_A2` (s) | `T_A3` (s) | `T_A4` (s) |
|---|---|---:|---:|---:|---:|
| [`Random100.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Random100.xlsx) | **MIN**<br>**MAX**<br>**MOYENNE**<br>**SOMME** | `0.000021160`<br>`0.003250000`<br>`0.001429318`<br>`0.142931808` | `0.000010815`<br>`0.001727273`<br>`0.000715686`<br>`0.071568627` | `0.000010711`<br>`0.001700000`<br>`0.000721126`<br>`0.072112645` | `0.000005270`<br>`0.000793651`<br>`0.000361426`<br>`0.036142630` |
| [`Random1000.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Random1000.xlsx) | **MIN**<br>**MAX**<br>**MOYENNE**<br>**SOMME** | `0.000000839`<br>`0.003642857`<br>`0.001585978`<br>`1.585977993` | `0.000000438`<br>`0.001645161`<br>`0.000787517`<br>`0.787517134` | `0.000000433`<br>`0.001785714`<br>`0.000789397`<br>`0.789396871` | `0.000000235`<br>`0.000852459`<br>`0.000397118`<br>`0.397117705` |
| [`Test-1.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Test-1.xlsx) | **MIN**<br>**MAX**<br>**MOYENNE**<br>**SOMME** | `0.000055804`<br>`0.003187500`<br>`0.001562969`<br>`0.156296899` | `0.000028653`<br>`0.001666667`<br>`0.000778929`<br>`0.077892943` | `0.000027949`<br>`0.001666667`<br>`0.000788660`<br>`0.078866002` | `0.000014331`<br>`0.000833333`<br>`0.000392721`<br>`0.039272125` |
| [`Test-2.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Test-2.xlsx) | **MIN**<br>**MAX**<br>**MOYENNE**<br>**SOMME** | `0.000000341`<br>`3.141000000`<br>`0.242007216`<br>`16.940505096` | `0.000000188`<br>`1.573000000`<br>`0.121536058`<br>`8.507524073` | `0.000000190`<br>`1.571000000`<br>`0.120991154`<br>`8.469380746` | `0.000000111`<br>`0.781000000`<br>`0.060414670`<br>`4.229026872` |
| [`Test-3.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Test-3.xlsx) | **MIN**<br>**MAX**<br>**MOYENNE**<br>**SOMME** | `0.000000359`<br>`2.831000000`<br>`0.577987888`<br>`17.339636645` | `0.000000192`<br>`1.412000000`<br>`0.287647543`<br>`8.629426301` | `0.000000193`<br>`1.434000000`<br>`0.289345739`<br>`8.680372168` | `0.000000118`<br>`0.702000000`<br>`0.144439852`<br>`4.333195566` |

---

## 4. Liste complète des Graphes dans `tests/graphes/` (Ce que chacun montre)

Tous les graphes possibles demandés par la prof et tes camarades sont maintenant générés dans `TP1_Primalite/tests/graphes/` :

### 1) Les graphes demandés question par question dans l'énoncé du TP1 :
- **[`TP1_Q3e_A1_temps_et_complexite.png`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/graphes/TP1_Q3e_A1_temps_et_complexite.png)** *(Question 3.e)* :
  - À gauche : la courbe du temps d'exécution $T(N)$ de `A1` en fonction de $N$ (c'est une droite parfaite).
  - À droite : la courbe de la complexité théorique au pire cas $O(N) = N-2$ en fonction de $N$ (la même droite parfaite !).
- **[`TP1_Comparaison_A1_A2.png`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/graphes/TP1_Comparaison_A1_A2.png)** *(Algorithme 2, Question 2)* :
  - Compare `A1` et `A2` : la droite de `A2` monte 2 fois moins vite que celle de `A1`.
- **[`TP1_Comparaison_A1_A2_A3.png`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/graphes/TP1_Comparaison_A1_A2_A3.png)** *(Algorithme 3, Question 2)* :
  - Compare `A1`, `A2` et `A3` : les droites de `A2` et `A3` sont confondues (toutes deux à la moitié de `A1`).
- **[`TP1_Comparaison_A1_A2_A3_A4.png`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/graphes/TP1_Comparaison_A1_A2_A3_A4.png)** *(Algorithme 4, Question 4)* :
  - Compare les 4 algorithmes en échelle linéaire : on voit très clairement que `A4` (rouge) est tout en bas (4 fois plus rapide que `A1` et 2 fois plus rapide que `A2`/`A3`).

### 2) Les graphes pour chacun des jeux de données (`Random100`, `Random1000`, `Test-1`, `Test-2`, `Test-3`, `Test-TP1`) :
- **`<Nom>_temps_vs_N.png`** : Axe $X = N$, Axe $Y = \text{Temps } T(N)$ des 4 algorithmes.
- **`<Nom>_min_moy_max.png`** : Axe $X = \text{Algorithme (`A1`, `A2`, `A3`, `A4`)}$, Axe $Y = \text{Temps MIN, MOYEN et MAX}$. On voit que les 3 courbes (`MIN`, `MOYEN`, `MAX`) descendent de `A1` vers `A4`.
- **`Test-2_par_chiffres.png` et `Test-3_par_chiffres.png`** : Axe $X = \text{Nombre de chiffres (3 à 9)}$, Axe $Y = \text{Temps moyen}$. Montre que le temps est multiplié par 10 à chaque chiffre supplémentaire.
