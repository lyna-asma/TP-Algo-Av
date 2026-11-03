# TP1 – Résumé Complet pour la Validation (Données, Code, Tests, Excel et Graphes)

> **Lis ce document de haut en bas : il t'explique 100 % du TP en français simple, étape par étape, avec les réponses exactes à donner à la professeure.**

---

## 1. Comprendre tout le TP en 2 minutes (La vue d'ensemble)

Le TP se déroule en **3 étapes** :

1. **Étape 1 : Les Données (`data/`)**
   - Le script Python ([`script-data.py`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/data/script-data.py)) génère des fichiers `.txt` remplis uniquement de **nombres premiers** (ex : `103`, `1000003`, `2048000011`...).
   - **Pourquoi uniquement des nombres premiers ?** Parce que pour vérifier qu'un nombre est premier, un algorithme cherche un diviseur. S'il n'en trouve aucun (cas d'un nombre premier), il est obligé de faire **tous les tours de boucle jusqu'à la fin**. C'est ce qu'on appelle tester le **pire cas**.

2. **Étape 2 : Le Code C (`code/tp1.c` et `tp1.exe`)**
   - Le programme C lit chaque nombre premier $N$ et chronomètre les **4 algorithmes** (`A1`, `A2`, `A3`, `A4`) en secondes :

| Algorithme | Ce qu'il fait dans [`tp1.c`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/code/tp1.c) | Nombre d'itérations au pire cas ($N$ premier) | Complexité théorique | Comparaison de performance |
|---|---|---:|:---:|---|
| **`A1`** | Teste tous les diviseurs $i$ de $2$ à $N-1$ (pas de 1) | $N - 2 \approx N$ | **$O(N)$** | Le plus lent (référence) |
| **`A2`** | Teste tous les diviseurs $i$ de $2$ à $N/2$ (pas de 1) | $N/2 - 1 \approx N/2$ | **$O(N)$** | **2 fois plus rapide que `A1`** |
| **`A3`** | Teste tous les diviseurs $i$ de $2$ à $\sqrt{N}$ (pas de 1) | $\sqrt{N} - 1 \approx \sqrt{N}$ | **$O(\sqrt{N})$** | **Incomparablement plus rapide** que `A1` et `A2` |
| **`A4`** | Élimine les pairs, puis teste uniquement les impairs $i$ de $3$ à $\sqrt{N}$ (pas de 2) | $\approx \sqrt{N} / 2$ | **$O(\sqrt{N})$** | **2 fois plus rapide que `A3`** (**le meilleur des 4 !**) |

3. **Étape 3 : Ta partie — Les Tests, les fichiers Excel et les Graphes (`tests/`)**
   - Les résultats des 6 jeux de données sont enregistrés dans **`tests/resultats/*.xlsx`** avec en bas de chaque colonne les formules Excel **`MIN`**, **`MAX`**, **`MOYENNE`** et **`SOMME`**.
   - Ton script [`tests/graphiques.py`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/graphiques.py) lit ces 6 fichiers `.xlsx` et trace les 18 graphes rangés dans tes 4 sous-dossiers de **`tests/graphes/`**.

---

## 2. Résultats et réponses sur l'échantillon imposé dans l'énoncé du TP1 (`Test-TP1.xlsx`)

### Question 3.a & 3.b : Tableau des mesures sur les 12 nombres premiers de la professeure ([`Test-TP1.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Test-TP1.xlsx))

| $N$ | Chiffres | Premier | $T_{A1}$ (s) | $T_{A2}$ (s) | $T_{A3}$ (s) | $T_{A4}$ (s) |
|---:|:---:|:---:|---:|---:|---:|---:|
| `1 000 003` | 7 | 1 | 0,003466667 | 0,001388889 | 0,000003563 | 0,000001606 |
| `2 000 003` | 7 | 1 | 0,006375000 | 0,003250000 | 0,000004785 | 0,000002441 |
| `4 000 037` | 7 | 1 | 0,012750000 | 0,012750000 | 0,000006570 | 0,000003555 |
| `8 000 009` | 7 | 1 | 0,025000000 | 0,012500000 | 0,000009393 | 0,000004637 |
| `16 000 057` | 8 | 1 | 0,062000000 | 0,024333333 | 0,000013453 | 0,000006714 |
| `32 000 011` | 8 | 1 | 0,104000000 | 0,051000000 | 0,000019380 | 0,000009416 |
| `64 000 031` | 8 | 1 | 0,206000000 | 0,102000000 | 0,000027223 | 0,000014925 |
| `128 000 003` | 9 | 1 | 0,412000000 | 0,208000000 | 0,000039063 | 0,000018797 |
| `256 000 001` | 9 | 1 | 0,821000000 | 0,415000000 | 0,000058824 | 0,000026978 |
| `512 000 009` | 9 | 1 | 1,654000000 | 0,821000000 | 0,000074257 | 0,000037313 |
| `1 024 000 009` | 10 | 1 | 3,270000000 | 1,654000000 | 0,000109489 | 0,000051903 |
| `2 048 000 011` | 10 | 1 | 6,619000000 | 3,305000000 | 0,000161290 | 0,000078534 |
| **MIN** | — | — | **0,003466667** | **0,001388889** | **0,000003563** | **0,000001606** |
| **MAX** | — | — | **6,619000000** | **3,305000000** | **0,000161290** | **0,000078534** |
| **MOYENNE** | — | — | **1,099632639** | **0,550851852** | **0,000043941** | **0,000021402** |
| **SOMME** | — | — | **13,195591667** | **6,610222222** | **0,000527290** | **0,000256819** |

### Réponses aux questions de l'énoncé du TP1 :
- **Question 3.c (Remarque sur l'échantillon et les mesures)** :
  - Chaque nombre $N$ est environ **le double** du précédent ($N_{k+1} \approx 2 \times N_k$).
  - Pour **`A1` et `A2`** (en $O(N)$) : chaque fois que $N$ double, le temps $T$ **double** aussi ($0{,}412\text{ s} \rightarrow 0{,}821\text{ s} \rightarrow 1{,}654\text{ s}$).
  - Pour **`A3` et `A4`** (en $O(\sqrt{N})$) : chaque fois que $N$ double, le temps est multiplié par $\sqrt{2} \approx 1{,}41$, et quand $N$ est multiplié par $4$ (toutes les 2 lignes), le temps **double** ($\sqrt{4N} = 2\sqrt{N}$).
- **Question 3.d (Compatibilité théorie vs expérience)** :
  - Les mesures expérimentales sont **100 % compatibles** avec les prédictions théoriques ($O(N)$ pour `A1`/`A2`, et $O(\sqrt{N})$ pour `A3`/`A4`).
- **Comparaison des 4 algorithmes** :
  - **`A2` vs `A1`** : `A2` est **2 fois plus rapide** que `A1` (`6,61 s` au total contre `13,20 s`).
  - **`A3` vs `A1` et `A2`** : `A3` passe d'une complexité $O(N)$ à **$O(\sqrt{N})$** : sur `2 048 000 011`, il met `0,00016 s` au lieu de `6,62 s` (plus de **40 000 fois plus rapide** !).
  - **`A4` vs `A3`** : **`A4` est le meilleur algorithme des 4**. Comme il ne teste que les diviseurs impairs jusqu'à $\sqrt{N}$, il fait $\sqrt{N}/2$ opérations et est **2 fois plus rapide que `A3`** (`0,000256 s` au total contre `0,000527 s`).

---

## 3. Résultats sur les 5 jeux de données du TP0 (`Random100`, `Random1000`, `Test-1`, `Test-2`, `Test-3`)

| Fichier Excel | Description | Statistique | `T_A1` (s) | `T_A2` (s) | `T_A3` (s) | `T_A4` (s) |
|---|---|---|---:|---:|---:|---:|
| [`Random100.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Random100.xlsx) | 100 premiers aléatoires ($\le 10^6$) | **MIN**<br>**MAX**<br>**MOYENNE**<br>**SOMME** | `0.000021160`<br>`0.003250000`<br>`0.001429318`<br>`0.142931808` | `0.000010815`<br>`0.001727273`<br>`0.000715686`<br>`0.071568627` | `0.000000291`<br>`0.000003911`<br>`0.000002135`<br>`0.000213472` | `0.000000164`<br>`0.000002397`<br>`0.000001084`<br>`0.000108429` |
| [`Random1000.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Random1000.xlsx) | 1000 premiers aléatoires ($\le 10^6$) | **MIN**<br>**MAX**<br>**MOYENNE**<br>**SOMME** | `0.000000839`<br>`0.003642857`<br>`0.001585978`<br>`1.585977993` | `0.000000438`<br>`0.001645161`<br>`0.000787517`<br>`0.787517134` | `0.000000073`<br>`0.000014199`<br>`0.000002338`<br>`0.002338164` | `0.000000062`<br>`0.000015814`<br>`0.000001198`<br>`0.001197655` |
| [`Test-1.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Test-1.xlsx) | 100 premiers dans $[10^3, 10^6]$ | **MIN**<br>**MAX**<br>**MOYENNE**<br>**SOMME** | `0.000055804`<br>`0.003187500`<br>`0.001562969`<br>`0.156296899` | `0.000028653`<br>`0.001666667`<br>`0.000778929`<br>`0.077892943` | `0.000000464`<br>`0.000008795`<br>`0.000002416`<br>`0.000241569` | `0.000000241`<br>`0.000002427`<br>`0.000001191`<br>`0.000119117` |
| [`Test-2.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Test-2.xlsx) | 10 premiers par taille de 3 à 12 chiffres (`A1`/`A2` mesurés jusqu'à 9 chiffres, **`A3`/`A4` jusqu'à 12 chiffres !**) | **MIN**<br>**MAX**<br>**MOYENNE**<br>**SOMME** | `0.000000341`<br>`3.141000000`<br>`0.242007216`<br>`16.940505096` | `0.000000188`<br>`1.573000000`<br>`0.121536058`<br>`8.507524073` | `0.000000063`<br>`0.003400000`<br>`0.000393043`<br>`0.039304344` | `0.000000050`<br>`0.001600000`<br>`0.000190451`<br>`0.019045073` |
| [`Test-3.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Test-3.xlsx) | 10 premiers à 3, 6, 9, 12 chiffres (`A1`/`A2` jusqu'à 9 chiffres, **`A3`/`A4` jusqu'à 12 chiffres !**) | **MIN**<br>**MAX**<br>**MOYENNE**<br>**SOMME** | `0.000000359`<br>`2.831000000`<br>`0.577987888`<br>`17.339636645` | `0.000000192`<br>`1.412000000`<br>`0.287647543`<br>`8.629426301` | `0.000000067`<br>`0.003200000`<br>`0.000656177`<br>`0.026247082` | `0.000000048`<br>`0.001666667`<br>`0.000332039`<br>`0.013281579` |

---

## 4. Comment expliquer tes 4 sous-dossiers de graphes (`tests/graphes/`)

1. **`comparaison-par-chiffres(taille-de-N)/`** (`Test-2_par_chiffres.png` et `Test-3_par_chiffres.png`) :
   - **C'est le graphe le plus visuel du TP !**
   - On voit les deux familles de complexité :
     - En haut, **`A1` (bleu) et `A2` (orange)** ont une pente raide ($O(N)$) et s'arrêtent à 9 chiffres (au-delà, ils prennent trop de temps).
     - En bas, **`A3` (vert) et `A4` (rouge)** ont une pente **deux fois plus douce** ($O(\sqrt{N})$) et continuent sans aucun problème jusqu'à **12 chiffres** (en à peine $0{,}001\text{ s}$ !).

2. **`comparaison-temps-vs-N(valeur-de-N)/`** (`*_temps_vs_N.png`) :
   - Axe $X = N$, Axe $Y = \text{Temps } T(N)$.
   - Montre les 4 courbes nettement séparées : `A1` au-dessus de `A2` (en haut), et `A3` au-dessus de `A4` (beaucoup plus bas).

3. **`min-moy-max/`** (`*_min_moy_max.png`) :
   - Axe $X = \text{Algorithme (`A1`, `A2`, `A3`, `A4`)}$, Axe $Y = \text{Temps MIN, MOYEN et MAX}$.
   - Montre la chute brutale du temps entre `A2` ($O(N)$) et `A3` ($O(\sqrt{N})$), puis la division par 2 entre `A3` et `A4`.

4. **`comparaison-graduelle/`** (`TP1_Comparaison_A1_A2.png`, `TP1_Comparaison_A1_A2_A3.png`, `TP1_Comparaison_A1_A2_A3_A4.png`) + `TP1_Q3e_A1_temps_et_complexite.png` :
   - Répondent dans l'ordre exact aux questions de l'énoncé du TP1.
