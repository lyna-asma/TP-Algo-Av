# Guide de Validation TP1 – Ce qu'il faut expliquer et dire à la professeure

---

## 1. L'idée générale du TP (Ce que tu expliques au début)

1. **Le problème** : On veut tester si un entier naturel $N$ est **premier** (divisible uniquement par $1$ et par lui-même).
2. **Pourquoi tous nos fichiers de données ne contiennent QUE des nombres premiers ?**
   - **Ce que tu dis à la prof** : *« Si un nombre n'est pas premier (par exemple un nombre pair), l'algorithme trouve tout de suite un diviseur dès le 1er tour et s'arrête : c'est le meilleur cas. En revanche, quand $N$ est premier, l'algorithme ne trouve aucun diviseur et est obligé de faire tous les tours de boucle jusqu'à la borne finale : cela nous permet de mesurer la complexité au **pire cas**. »*
3. **Les 4 algorithmes (`A1`, `A2`, `A3`, `A4`) dans [`tp1.c`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/code/tp1.c)** :

| Algorithme | Diviseurs testés | Nombre de tours au pire cas ($N$ premier) | Complexité au pire cas | Vitesse |
|---|---|---|---|---|
| **`A1`** | Tous les entiers $i$ de $2$ à $N-1$ (pas de 1) | $N - 2 \approx N$ | **$O(N)$** (linéaire) | Le plus lent (référence) |
| **`A2`** | Tous les entiers $i$ de $2$ à $N/2$ (pas de 1) | $N/2 - 1 \approx N/2$ | **$O(N)$** (linéaire) | **2 fois plus rapide que `A1`** |
| **`A3`** | Tous les entiers $i$ de $2$ à $\sqrt{N}$ (pas de 1) | $\sqrt{N} - 1 \approx \sqrt{N}$ | **$O(\sqrt{N})$** (racine) | **Des milliers de fois plus rapide que `A1` et `A2`** |
| **`A4`** | Teste si $N$ est pair ; sinon teste uniquement les **impairs** $i$ de $3$ à $\sqrt{N}$ (pas de 2) | $\approx \sqrt{N} / 2$ | **$O(\sqrt{N})$** (racine) | **2 fois plus rapide que `A3` (le meilleur des 4 !)** |

---

## 2. Les 6 jeux de données (`data/*.txt`) et leurs fichiers Excel (`tests/resultats/*.xlsx`)

Dans chaque fichier `.xlsx`, on a pour chaque nombre $N$ les temps `T_A1`, `T_A2`, `T_A3`, `T_A4`, et en bas du tableau 4 lignes calculées avec les formules d'Excel : **`MIN`**, **`MAX`**, **`MOYENNE`** et **`SOMME`**.

1. **[`Test-TP1.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Test-TP1.xlsx)** *(Les 12 nombres du tableau de l'énoncé du TP1 : `1000003` à `2048000011`)* :
   - **Particularité** : Chaque nombre $N$ est **le double** du précédent ($N_{k+1} \approx 2 \times N_k$).
   - **Ce que tu dis à la prof (Questions 3.c et 3.d)** :
     - *« Pour `A1` et `A2` qui sont en $O(N)$, chaque fois que $N$ double d'une ligne à la suivante, le temps $T(N)$ **double** aussi (ex : `0,41 s` $\rightarrow$ `0,82 s` $\rightarrow$ `1,65 s` $\rightarrow$ `3,27 s` $\rightarrow$ `6,62 s`). »*
     - *« Pour `A3` et `A4` qui sont en $O(\sqrt{N})$, quand $N$ est multiplié par 4 (toutes les 2 lignes), le temps est multiplié par $\sqrt{4} = 2$. Et sur le plus grand nombre (`2 048 000 011`), `A1` met `6,62 s` alors que `A3` met `0,00016 s` et `A4` met `0,000078 s` ! Les mesures expérimentales sont donc 100 % compatibles avec la complexité théorique. »*

2. **[`Random100.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Random100.xlsx), [`Random1000.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Random1000.xlsx) et [`Test-1.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Test-1.xlsx)** *(Jeux 1, 2 et 3 du TP0 — aléatoires $\le 10^6$)* :
   - **Ce que tu dis à la prof** : *« Dans ces 3 jeux, les nombres sont tirés au hasard jusqu'à $10^6$. Comme la majorité des nombres premiers inférieurs à $10^6$ ont 6 chiffres (entre $100\,000$ et $1\,000\,000$), le tirage aléatoire produit surtout des nombres de 6 chiffres et très peu de petits nombres. »*

3. **[`Test-2.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Test-2.xlsx) et [`Test-3.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Test-3.xlsx)** *(Jeux 4 et 5 du TP0 — contrôlés par nombre de chiffres de 3 à 12 chiffres)* :
   - **Ce que tu dis à la prof** : *« Ici, on a contrôlé l'échantillon en prenant exactement 10 nombres premiers par taille (de 3 à 12 chiffres dans `Test-2`, et 3, 6, 9, 12 chiffres dans `Test-3`). Au-delà de 9 chiffres ($N > 2{,}1 \times 10^9$), `A1` et `A2` en $O(N)$ sont trop lents (marqués `-` dans Excel), tandis que **`A3` et `A4` en $O(\sqrt{N})$ calculent même les nombres à 12 chiffres en à peine 1 milliseconde !** »*

---

## 3. Ce que représente chacun de tes 4 sous-dossiers de `tests/graphes/` et ce que tu dis à la prof

### Sous-dossier 1 : `comparaison-graduelle/` + `TP1_Q3e_A1_temps_et_complexite.png`
*(Répond dans l'ordre aux questions de l'énoncé du TP1 sur l'échantillon des 12 valeurs)*

1. **[`TP1_Q3e_A1_temps_et_complexite.png`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/graphes/TP1_Q3e_A1_temps_et_complexite.png)** *(Question 3.e)* :
   - **Ce qu'il représente** : À gauche, le temps mesuré $T(N)$ de `A1` en fonction de $N$ ; à droite, la complexité théorique au pire cas $O(N) = N-2$ en fonction de $N$.
   - **Ce que tu dis à la prof** : *« On obtient deux droites linéaires identiques : cela montre que le temps d'exécution réel $T(N)$ suit exactement la loi théorique $O(N)$. »*
2. **[`TP1_Comparaison_A1_A2.png`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/graphes/comparaison-graduelle/TP1_Comparaison_A1_A2.png)** *(Algorithme 2 – Question 2)* :
   - **Ce qu'il représente** : $X = N$, $Y = \text{Temps } T(N)$ (en échelle linéaire) pour `A1` et `A2`.
   - **Ce que tu dis à la prof** : *« Les deux courbes sont des droites car `A1` et `A2` sont tous les deux en $O(N)$, mais la pente de `A2` (orange) est exactement la moitié de celle de `A1` (bleue) car `A2` s'arrête à $N/2$. `A2` est donc 2 fois plus performant que `A1`. »*
3. **[`TP1_Comparaison_A1_A2_A3.png`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/graphes/comparaison-graduelle/TP1_Comparaison_A1_A2_A3.png)** et **[`TP1_Comparaison_A1_A2_A3_A4.png`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/graphes/comparaison-graduelle/TP1_Comparaison_A1_A2_A3_A4.png)** *(Algorithmes 3 et 4)* :
   - **Ce qu'ils représentent** : $X = N$, $Y = \text{Temps } T(N)$ (en échelle logarithmique).
   - **Ce que tu dis à la prof** : *« On utilise une échelle logarithmique car `A3` et `A4` sont des dizaines de milliers de fois plus rapides que `A1` et `A2` (en échelle normale, `A3` et `A4` seraient écrasés sur l'axe $Y=0$). On voit deux familles bien séparées : en haut `A1` et `A2` en $O(N)$ (pente forte), et tout en bas `A3` et `A4` en $O(\sqrt{N})$ (pente deux fois plus faible). Et la courbe rouge de `A4` est sous la courbe verte de `A3` car `A4` ne teste que les impairs : c'est **`A4` le meilleur algorithme des 4**. »*

---

### Sous-dossier 2 : `comparaison-temps-vs-N(valeur-de-N)/` (`*_temps_vs_N.png`)
- **Axe $X$** : le nombre premier $N$ testé (trié du plus petit au plus grand).
- **Axe $Y$** : le temps d'exécution $T(N)$ en secondes (échelle log-log).
- **Ce que tu dis à la prof** :
  - *« Sur les 6 jeux de données, la hiérarchie est toujours la même du plus lent au plus rapide : `A1` (bleu, tout en haut) > `A2` (orange) $\gg$ `A3` (vert) > `A4` (rouge, tout en bas). »*
  - *« En comparant [`Random100_temps_vs_N.png`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/graphes/comparaison-temps-vs-N(valeur-de-N)/Random100_temps_vs_N.png) avec [`Test-2_temps_vs_N.png`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/graphes/comparaison-temps-vs-N(valeur-de-N)/Test-2_temps_vs_N.png), on voit l'intérêt du TP0 : dans `Random100`, les points sont concentrés vers $10^5..10^6$, alors que dans `Test-2` ils couvrent régulièrement tous les ordres de grandeur de $10^2$ à $10^{12}$, et seules les courbes verte (`A3`) et rouge (`A4`) peuvent aller jusqu'à $10^{12}$. »*

---

### Sous-dossier 3 : `comparaison-par-chiffres(taille-de-N)/` ([`Test-2_par_chiffres.png`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/graphes/comparaison-par-chiffres(taille-de-N)/Test-2_par_chiffres.png) et [`Test-3_par_chiffres.png`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/graphes/comparaison-par-chiffres(taille-de-N)/Test-3_par_chiffres.png))
- **Axe $X$** : le **nombre de chiffres** de $N$ (de 3 chiffres à 12 chiffres).
- **Axe $Y$** : le **temps moyen** d'exécution pour les 10 nombres de cette taille.
- **Ce que tu dis à la prof** :
  - *« C'est le graphe qui illustre le mieux la différence entre $O(N)$ et $O(\sqrt{N})$ : »*
  - *« Quand on ajoute **1 chiffre** à $N$, $N$ est multiplié par $10$ : pour `A1` et `A2` (en $O(N)$), le temps est multiplié par **10** à chaque chiffre (et au-delà de 9 chiffres, ils dépassent la limite). »*
  - *« En revanche, pour `A3` et `A4` (en $O(\sqrt{N})$), quand on ajoute **2 chiffres** ($N \times 100$), $\sqrt{100} = 10$, donc leur temps n'est multiplié par 10 que tous les **2 chiffres** ! Leur pente est donc **deux fois plus faible**, et même à **12 chiffres**, `A3` et `A4` prennent moins de `0,003 s`. »*

---

### Sous-dossier 4 : `min-moy-max/` (`*_min_moy_max.png`)
- **Axe $X$** : les 4 algorithmes (`A1`, `A2`, `A3`, `A4`).
- **Axe $Y$** : le temps en secondes.
- **Les 3 courbes** :
  - Verte (en haut) = **Temps MAX** de chaque algorithme,
  - Orange (au milieu) = **Temps MOYEN** de chaque algorithme,
  - Bleue (en bas) = **Temps MIN** de chaque algorithme.
- **Ce que tu dis à la prof** :
  - *« Ce graphe résume les lignes `MIN`, `MOYENNE` et `MAX` calculées en bas de nos tableaux Excel : on voit une légère baisse de `A1` à `A2` ($\div 2$), puis une **chute énorme** de `A2` à `A3` (passage de $O(N)$ à $O(\sqrt{N})$), et enfin une nouvelle baisse de `A3` à `A4` ($\div 2$ grâce à l'élimination des diviseurs pairs). »*

---

## 4. Tableaux récapitulatifs des valeurs (`Test-TP1` et les 5 jeux du TP0)

### A. Les 12 valeurs de l'énoncé du TP1 ([`Test-TP1.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Test-TP1.xlsx))

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

### B. Résumé `MIN`, `MAX`, `MOYENNE`, `SOMME` des 6 fichiers Excel

| Fichier Excel | Statistique | `T_A1` (s) | `T_A2` (s) | `T_A3` (s) | `T_A4` (s) |
|---|---|---:|---:|---:|---:|
| **[`Test-TP1.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Test-TP1.xlsx)** *(12 nombres du TP1)* | **MIN**<br>**MAX**<br>**MOYENNE**<br>**SOMME** | `0.003466667`<br>`6.619000000`<br>`1.099632639`<br>`13.195591667` | `0.001388889`<br>`3.305000000`<br>`0.550851852`<br>`6.610222222` | `0.000003563`<br>`0.000161290`<br>`0.000043941`<br>`0.000527290` | `0.000001606`<br>`0.000078534`<br>`0.000021402`<br>`0.000256819` |
| **[`Random100.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Random100.xlsx)** | **MIN**<br>**MAX**<br>**MOYENNE**<br>**SOMME** | `0.000021160`<br>`0.003250000`<br>`0.001429318`<br>`0.142931808` | `0.000010815`<br>`0.001727273`<br>`0.000715686`<br>`0.071568627` | `0.000000291`<br>`0.000003911`<br>`0.000002135`<br>`0.000213472` | `0.000000164`<br>`0.000002397`<br>`0.000001084`<br>`0.000108429` |
| **[`Random1000.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Random1000.xlsx)** | **MIN**<br>**MAX**<br>**MOYENNE**<br>**SOMME** | `0.000000839`<br>`0.003642857`<br>`0.001585978`<br>`1.585977993` | `0.000000438`<br>`0.001645161`<br>`0.000787517`<br>`0.787517134` | `0.000000073`<br>`0.000014199`<br>`0.000002338`<br>`0.002338164` | `0.000000062`<br>`0.000015814`<br>`0.000001198`<br>`0.001197655` |
| **[`Test-1.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Test-1.xlsx)** | **MIN**<br>**MAX**<br>**MOYENNE**<br>**SOMME** | `0.000055804`<br>`0.003187500`<br>`0.001562969`<br>`0.156296899` | `0.000028653`<br>`0.001666667`<br>`0.000778929`<br>`0.077892943` | `0.000000464`<br>`0.000008795`<br>`0.000002416`<br>`0.000241569` | `0.000000241`<br>`0.000002427`<br>`0.000001191`<br>`0.000119117` |
| **[`Test-2.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Test-2.xlsx)** | **MIN**<br>**MAX**<br>**MOYENNE**<br>**SOMME** | `0.000000341`<br>`3.141000000`<br>`0.242007216`<br>`16.940505096` | `0.000000188`<br>`1.573000000`<br>`0.121536058`<br>`8.507524073` | `0.000000063`<br>`0.003400000`<br>`0.000393043`<br>`0.039304344` | `0.000000050`<br>`0.001600000`<br>`0.000190451`<br>`0.019045073` |
| **[`Test-3.xlsx`](file:///c:/Users/lyna/Desktop/M1-Semestre1/TP-Algo-Av/TP1_Primalite/tests/resultats/Test-3.xlsx)** | **MIN**<br>**MAX**<br>**MOYENNE**<br>**SOMME** | `0.000000359`<br>`2.831000000`<br>`0.577987888`<br>`17.339636645` | `0.000000192`<br>`1.412000000`<br>`0.287647543`<br>`8.629426301` | `0.000000067`<br>`0.003200000`<br>`0.000656177`<br>`0.026247082` | `0.000000048`<br>`0.001666667`<br>`0.000332039`<br>`0.013281579` |
