# TP1 – Analyse des Tests, Résultats Excel et Interprétation des Graphes

> Ce document regroupe tout ce qu'il faut savoir pour présenter et expliquer la **partie Tests** du TP1 devant la professeure :
> 1. Les jeux de données testés (lien entre TP0 et TP1)
> 2. Les résultats Excel (`MIN`, `MAX`, `MOYENNE`, `SOMME`) et réponses aux questions du TP1
> 3. Comment lire et interpréter les 6 graphes (`tests/graphes/*.png`)
> 4. Explication simple du script [`graphiques.py`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/graphiques.py)

---

## 1. Les 4 algorithmes testés (Rappel rapide)

Tous les nombres testés dans nos fichiers sont des **nombres premiers**, ce qui correspond au **pire cas** (la boucle va jusqu'au bout sans trouver de diviseur) :

| Algorithme | Principe (diviseurs testés) | Nombre d'itérations au pire cas ($N$ premier) | Complexité théorique | Gain théorique par rapport à `A1` |
|---|---|---:|:---:|---|
| **`A1`** | Teste tous les entiers $i$ de $2$ à $N-1$ (pas de 1) | $N - 2 \approx N$ | $O(N)$ | Référence ($1\times$) |
| **`A2`** | Teste tous les entiers $i$ de $2$ à $N/2$ (pas de 1) | $N/2 - 1 \approx N/2$ | $O(N)$ | **$2\times$ plus rapide** que `A1` |
| **`A3`** | Élimine les pairs, puis teste les impairs $i$ de $3$ à $N-2$ (pas de 2) | $(N - 3)/2 \approx N/2$ | $O(N)$ | **$2\times$ plus rapide** que `A1` (identique à `A2`) |
| **`A4`** | Élimine les pairs, puis teste les impairs $i$ de $3$ à $N/2$ (pas de 2) | $\approx N/4$ | $O(N)$ | **$4\times$ plus rapide** que `A1` (**le meilleur**) |

---

## 2. Partie A — Résultats et réponses sur l'échantillon imposé dans l'énoncé du TP1 (`Test-TP1.xlsx`)

### Question 3.a & 3.b : Vérification de la primalité et tableau des temps d'exécution $T(N)$
Les 12 nombres proposés dans le tableau de l'énoncé du TP1 ont tous été vérifiés premiers (`premier = 1`). Voici les temps mesurés dans [`Test-TP1.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Test-TP1.xlsx) :

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

### Question 3.c : Que remarque-t-on sur les données de l'échantillon et sur les mesures obtenues ?
1. **Sur les données $N$** : Chaque nombre $N$ de l'échantillon est approximativement **le double** du nombre précédent ($N_{k+1} \approx 2 \times N_k$, par exemple $128\,000\,003 \rightarrow 256\,000\,001 \rightarrow 512\,000\,009$).
2. **Sur les mesures de temps $T$** : D'une valeur à la suivante, le temps d'exécution $T$ **double également** ($T_{k+1} \approx 2 \times T_k$). Par exemple pour `A1` : $0{,}412\text{ s} \rightarrow 0{,}821\text{ s} \rightarrow 1{,}654\text{ s} \rightarrow 3{,}270\text{ s} \rightarrow 6{,}619\text{ s}$.

### Question 3.d : Comparer la complexité théorique et les mesures expérimentales. Sont-elles compatibles ?
- **Oui, elles sont parfaitement compatibles.**
- La complexité théorique au pire cas est **linéaire en $O(N)$** : le temps d'exécution théorique est de la forme $T(N) \approx c \times N$.
- Par conséquent, si on multiplie $N$ par $2$, la théorie prédit que $T(2N) = 2\,T(N)$ (le temps double), ce qui est exactement ce que l'on observe expérimentalement.

### Comparaison entre les 4 algorithmes (`A1`, `A2`, `A3`, `A4`) :
- **`A1` vs `A2`** : `A2` est **2 fois plus rapide** que `A1` (temps moyen `0,55 s` contre `1,10 s`), car il s'arrête à $N/2$.
- **`A2` vs `A3`** : `A3` a quasiment **la même performance** que `A2` (temps moyen `0,548 s` contre `0,551 s`). En effet, sur un nombre premier impair, tester uniquement les diviseurs impairs jusqu'à $N-2$ (pas de 2) demande $(N-3)/2 \approx N/2$ tours, soit autant de tours que `A2`.
- **`A4` vs tous les autres** : **`A4` est le meilleur algorithme (le plus performant)**. En combinant l'arrêt à $N/2$ et le pas de 2 (impairs seulement), il ne fait que $N/4$ itérations et est **4 fois plus rapide que `A1`** (temps moyen `0,27 s` contre `1,10 s`).

---

## 3. Partie B — Résultats sur les 5 jeux de données du TP0 (`Random100`, `Random1000`, `Test-1`, `Test-2`, `Test-3`)

### Tableau récapitulatif des statistiques (`MIN`, `MAX`, `MOYENNE`, `SOMME`)

| Fichier Excel | Description du jeu de données | Statistique | `T_A1` (s) | `T_A2` (s) | `T_A3` (s) | `T_A4` (s) |
|---|---|---|---|---:|---:|---:|---:|
| [`Random100.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Random100.xlsx) | 100 premiers aléatoires ($\le 10^6$) | **MIN**<br>**MAX**<br>**MOYENNE**<br>**SOMME** | `0.000021160`<br>`0.003250000`<br>`0.001429318`<br>`0.142931808` | `0.000010815`<br>`0.001727273`<br>`0.000715686`<br>`0.071568627` | `0.000010711`<br>`0.001700000`<br>`0.000721126`<br>`0.072112645` | `0.000005270`<br>`0.000793651`<br>`0.000361426`<br>`0.036142630` |
| [`Random1000.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Random1000.xlsx) | 1000 premiers aléatoires ($\le 10^6$) | **MIN**<br>**MAX**<br>**MOYENNE**<br>**SOMME** | `0.000000839`<br>`0.003642857`<br>`0.001585978`<br>`1.585977993` | `0.000000438`<br>`0.001645161`<br>`0.000787517`<br>`0.787517134` | `0.000000433`<br>`0.001785714`<br>`0.000789397`<br>`0.789396871` | `0.000000235`<br>`0.000852459`<br>`0.000397118`<br>`0.397117705` |
| [`Test-1.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Test-1.xlsx) | 100 premiers dans $[10^3, 10^6]$ | **MIN**<br>**MAX**<br>**MOYENNE**<br>**SOMME** | `0.000055804`<br>`0.003187500`<br>`0.001562969`<br>`0.156296899` | `0.000028653`<br>`0.001666667`<br>`0.000778929`<br>`0.077892943` | `0.000027949`<br>`0.001666667`<br>`0.000788660`<br>`0.078866002` | `0.000014331`<br>`0.000833333`<br>`0.000392721`<br>`0.039272125` |
| [`Test-2.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Test-2.xlsx) | 10 premiers par taille de 3 à 12 chiffres (mesurés de 3 à 9 chiffres) | **MIN**<br>**MAX**<br>**MOYENNE**<br>**SOMME** | `0.000000341`<br>`3.141000000`<br>`0.242007216`<br>`16.940505096` | `0.000000188`<br>`1.573000000`<br>`0.121536058`<br>`8.507524073` | `0.000000190`<br>`1.571000000`<br>`0.120991154`<br>`8.469380746` | `0.000000111`<br>`0.781000000`<br>`0.060414670`<br>`4.229026872` |
| [`Test-3.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Test-3.xlsx) | 10 premiers à 3, 6, 9, 12 chiffres (mesurés à 3, 6, 9 chiffres) | **MIN**<br>**MAX**<br>**MOYENNE**<br>**SOMME** | `0.000000359`<br>`2.831000000`<br>`0.577987888`<br>`17.339636645` | `0.000000192`<br>`1.412000000`<br>`0.287647543`<br>`8.629426301` | `0.000000193`<br>`1.434000000`<br>`0.289345739`<br>`8.680372168` | `0.000000118`<br>`0.702000000`<br>`0.144439852`<br>`4.333195566` |

### Conclusion sur l'impact du choix des données (Objectif du TP0) :
1. **`Random100`, `Random1000` et `Test-1`** :
   - Quand on tire des nombres premiers au hasard jusqu'à $10^6$, ~90 % des nombres tirés ont **6 chiffres** (entre $100\,000$ et $1\,000\,000$).
   - Sur les graphes [`Random100.png`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/graphes/Random100.png), [`Random1000.png`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/graphes/Random1000.png) et [`Test-1.png`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/graphes/Test-1.png), on voit que les points sont très concentrés à droite près de $10^6$ et très rares sur les petits nombres. Un tirage purement aléatoire représente donc mal les différentes échelles de grandeur.
2. **`Test-2` et `Test-3` (Échantillons contrôlés par nombre de chiffres)** :
   - En imposant 10 nombres par nombre de chiffres (3, 4, 5, ..., 9 chiffres dans [`Test-2.png`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/graphes/Test-2.png), et 3, 6, 9 chiffres dans [`Test-3.png`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/graphes/Test-3.png)), les points sont **parfaitement répartis** sur tout l'axe horizontal.
   - On voit très nettement que chaque fois que $N$ gagne 1 chiffre ($\times 10$), le temps $T(N)$ est multiplié par 10, et qu'au-delà de 9 chiffres ($N > 2{,}1 \times 10^9$), les algorithmes en $O(N)$ deviennent trop lents (plusieurs dizaines de secondes à plusieurs heures par nombre pour 12 chiffres).

---

## 4. Comment expliquer les graphes (`tests/graphes/*.png`) devant la professeure

Sur chacun des 6 graphes (`Test-TP1.png`, `Random100.png`, `Random1000.png`, `Test-1.png`, `Test-2.png`, `Test-3.png`) :
- **Axe horizontal $X$ (abscisse)** : le nombre premier **$N$** testé.
- **Axe vertical $Y$ (ordonnée)** : le **temps d'exécution $T(N)$ en secondes**.
- **Pourquoi une échelle logarithmique (`log`) sur $X$ et $Y$ ?**
  Parce que $N$ varie de $100$ ($10^2$) à $2\,048\,000\,011$ ($\approx 2 \times 10^9$), et le temps varie de $0{,}0000001\text{ s}$ ($10^{-7}\text{ s}$) à $6{,}6\text{ s}$. Sans échelle logarithmique, tous les nombres inférieurs à $10^7$ seraient écrasés à $0$ en bas à gauche du graphique.
- **Ce qu'on voit sur les 4 courbes :**
  1. Les 4 courbes forment des **droites parallèles de pente 1** en échelle log-log, ce qui confirme qu'elles ont toutes la même classe de complexité asymptotique linéaire **$O(N)$**.
  2. La courbe **bleue (`A1`)** est tout en haut (la plus lente).
  3. Les courbes **orange (`A2`)** et **verte (`A3`)** sont **confondues au milieu** : elles font toutes les deux $N/2$ divisions et prennent la moitié du temps de `A1`.
  4. La courbe **rouge (`A4`)** est tout en bas : c'est la plus rapide (le quart du temps de `A1`).

