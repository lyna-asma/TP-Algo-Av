**USTHB — Faculté d'Électronique et d'Informatique — Département d'Informatique**  
**Master 1 : Réseaux et Systèmes Distribués (RSD) — Module : Algorithmique Avancée et Complexité**

# TP N°03 : Complexité Polynomiale

- **Groupe :** 13 | **Année universitaire :** 2026 / 2027 | **Enseignante :** Mme MESSAOUDI
- **Membres :** ADJAL Lyna Asma *(Rapport)* · MENADI Maria *(Code)* · KHERIS Aya *(Données)* · BOUSSETOUA Rania *(Tests)*

---

## Exercice 1 : Produit de deux matrices

Soient deux matrices *A(n, m)* et *B(m, p)* (*n, m, p* ≥ 1) et leur produit *C(n, p) = A(n, m) × B(m, p)*.

### 1. Programme C du produit matriciel

Chaque élément est calculé par `C(i, j) = ∑_{k=0..m−1} A(i, k) × B(k, j)` pour `i ∈ [0, n−1]` et `j ∈ [0, p−1]`. Les matrices sont stockées en tableaux 1D contigus (`M[i][j] ↔ M[i * nb_colonnes + j]`).

```c
void produitMatrices(int n, int m, int p, const int *A, const int *B, int *C)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p; j++) {
            int somme = 0;
            for (int k = 0; k < m; k++) {
                somme += A[i * m + k] * B[k * p + j];
            }
            C[i * p + j] = somme;
        }
    }
}

void produitMatricesCarrees(int n, const int *A, const int *B, int *C)
{
    produitMatrices(n, n, n, A, B, C);
}
```

### 2. Complexité temporelle théorique

- **Cas général (*A(n, m) × B(m, p)*) :** Les trois boucles imbriquées (`i` de `0` à `n−1`, `j` de `0` à `p−1`, `k` de `0` à `m−1`) sans condition d'arrêt exécutent exactement `n × m × p` itérations internes (soit `n × m × p` multiplications et `n × m × p` additions).  
  **Complexité :** ***O(n × m × p)*** (meilleur cas = pire cas).
- **Cas des matrices carrées (*n = m = p*) :** Le nombre d'opérations devient proportionnel à `n³`.  
  **Complexité :** ***O(n³)*** (complexité cubique).

### 3. Espace mémoire nécessaire

Chaque entier `int` occupe `4` octets (`sizeof(int) = 4`) :

- **Matrice *A(n, m)* :** `4 × n × m` octets
- **Matrice *B(m, p)* :** `4 × m × p` octets
- **Matrice *C(n, p)* :** `4 × n × p` octets
- **Variables locales (`i, j, k, somme`, paramètres) :** *O(1)*

- **Cas général :** **`S(n, m, p) = 4(nm + mp + np) + O(1)` octets**, soit ***O(nm + mp + np)*** (mémoire auxiliaire seule de la fonction : *O(1)*).
- **Cas des matrices carrées (*n = m = p*) :** **`S(n) = 12n² + O(1)` octets**, soit ***O(n²)*** *(ex. 0,11 Mo pour n = 100 ; 11,44 Mo pour n = 1000)*.

### 4. Mesures des temps d'exécution *T(n)* (matrices carrées *n × n*)

| Taille *n* | Opérations *n³* | Temps *T(n)* (ms) | Temps *T(n)* (s) | Ratio *T(n) / n³* |
| :---: | :---: | :---: | :---: | :---: |
| **100** | 1 000 000 | `[À compléter]` | `[À compléter]` | `[À compléter]` |
| **200** | 8 000 000 | `[À compléter]` | `[À compléter]` | `[À compléter]` |
| **400** | 64 000 000 | `[À compléter]` | `[À compléter]` | `[À compléter]` |
| **600** | 216 000 000 | `[À compléter]` | `[À compléter]` | `[À compléter]` |
| **800** | 512 000 000 | `[À compléter]` | `[À compléter]` | `[À compléter]` |
| **1000** | 1 000 000 000 | `[À compléter]` | `[À compléter]` | `[À compléter]` |

### 5. Graphe des variations du temps *T* par rapport à *n*

> `[Insérer ici le graphe T(n) en fonction de n]`

### 6. Comparaison entre complexité théorique et complexité expérimentale

`[À compléter après les tests :]`
- Vérifier que les rapports de doublement `T(2n) / T(n)` restent proches de `2³ = 8` (par ex. `T(400)/T(200) = [...]`, `T(800)/T(400) = [...]`) et que `T(n) / n³` se stabilise pour les grandes tailles.
- Noter d'éventuels écarts dus à la granularité de `clock()` pour les petites matrices (`n = 100, 200`) et aux défauts de cache lors du parcours en colonne de `B` (`B[k * p + j]`) pour les grandes matrices (`n ≥ 600`).

---

## Exercice 2 : Recherche d'une sous-matrice

Soient *A(n, m)* et *B(n', m')* avec *n' < n* et *m' < m*.

### 1. Matrices non triées : fonction `sousMat1` et complexité théorique

**Principe :** Parcours de toutes les positions valides `(i, j)` (`0 ≤ i ≤ n − n'`, `0 ≤ j ≤ m − m'`). Pour chaque position, comparaison bloc par bloc de taille `n' × m'` avec arrêt immédiat (`break`) dès la première valeur différente.

```c
int sousMat1(int n, int m, const int *A, 
             int n_prime, int m_prime, const int *B, 
             int *pos_i, int *pos_j)
{
    int max_i = n - n_prime;
    int max_j = m - m_prime;

    for (int i = 0; i <= max_i; i++) {
        for (int j = 0; j <= max_j; j++) {
            int correspond = 1;
            for (int di = 0; di < n_prime; di++) {
                for (int dj = 0; dj < m_prime; dj++) {
                    if (A[(i + di) * m + (j + dj)] != B[di * m_prime + dj]) {
                        correspond = 0;
                        break;
                    }
                }
                if (!correspond) break;
            }
            if (correspond) {
                if (pos_i) *pos_i = i;
                if (pos_j) *pos_j = j;
                return 1;
            }
        }
    }
    return 0;
}
```

**Complexité temporelle théorique :**
- **Meilleur cas :** ***O(n' × m')*** (*B* trouvée directement en `(0, 0)`).
- **Pire cas théorique absolu :** ***O((n − n' + 1)(m − m' + 1) × n' × m')*** (cas carré : *O((n − k + 1)² × k²)*).
- **Comportement pratique avec `break` (données aléatoires, *B* en fin de matrice) :** Rejet en *O(1)* sur les positions précédentes, soit ***O((n − n' + 1)(m − m' + 1) + n' × m')***.

### 2. Lignes triées : fonction `sousMat2` et complexité théorique

**Principe :** Chaque ligne de *A* et de *B* étant triée par ordre croissant :
1. **Élagage en *O(1)* :** Si `B[0][0] < A[i][0]` ou `B[0][m'−1] > A[i][m−1]`, la ligne `i` est immédiatement ignorée.
2. **Recherche dichotomique en *O(log₂(m − m' + 1))* :** Localisation de la première occurrence de `B[0][0]` sur la ligne `i` via `dicho_premiere_occurrence`.
3. **Vérification ciblée (avec gestion des doublons) :** Test rapide des deux extrémités de chaque ligne du bloc avant de vérifier les éléments intérieurs.

```c
static int dicho_premiere_occurrence(const int *T, int taille_max, int cible)
{
    int g = 0, d = taille_max - 1;
    int res = -1;
    while (g <= d) {
        int mid = (g + d) / 2;
        if (T[mid] == cible) {
            res = mid;
            d = mid - 1;
        } else if (T[mid] < cible) {
            g = mid + 1;
        } else {
            d = mid - 1;
        }
    }
    return res;
}

int sousMat2(int n, int m, const int *A, 
             int n_prime, int m_prime, const int *B, 
             int *pos_i, int *pos_j)
{
    int max_i = n - n_prime;
    int max_j = m - m_prime;
    int cible = B[0];
    int dernier_B0 = B[m_prime - 1];

    for (int i = 0; i <= max_i; i++) {
        const int *ligne_A = &A[i * m];

        if (ligne_A[0] > cible || ligne_A[m - 1] < dernier_B0) {
            continue;
        }

        int j = dicho_premiere_occurrence(ligne_A, max_j + 1, cible);

        while (j != -1 && j <= max_j && ligne_A[j] == cible) {
            int correspond = 1;
            for (int di = 0; di < n_prime; di++) {
                const int *ligne_Adi = &A[(i + di) * m];
                const int *ligne_Bdi = &B[di * m_prime];

                if (ligne_Adi[j] != ligne_Bdi[0] || 
                    ligne_Adi[j + m_prime - 1] != ligne_Bdi[m_prime - 1]) {
                    correspond = 0;
                    break;
                }
                for (int dj = 1; dj < m_prime - 1; dj++) {
                    if (ligne_Adi[j + dj] != ligne_Bdi[dj]) {
                        correspond = 0;
                        break;
                    }
                }
                if (!correspond) break;
            }
            if (correspond) {
                if (pos_i) *pos_i = i;
                if (pos_j) *pos_j = j;
                return 1;
            }
            j++;
        }
    }
    return 0;
}
```

**Complexité temporelle théorique :**
- **Meilleur cas :** ***O(n − n' + 1)*** (toutes les lignes élaguées en *O(1)*) ou ***O(log₂(m − m' + 1) + n' × m')*** (*B* trouvée sur la ligne 0).
- **Cas nominal (doublons bornés par ligne) :** ***O((n − n' + 1) × log₂(m − m' + 1) + n' × m')*** (cas carré : *O((n − k + 1) × log₂(n − k + 1) + k²)*). Gain d'un facteur `~ m / log₂ m` sur le balayage par rapport à `sousMat1`.
- **Cas dégénéré (matrice constante pleine de doublons) :** Retombe à *O((n − n' + 1)(m − m' + 1) × n' × m')* *(reste strictement borné par O((n − n' + 1)(log₂ m + n' × m')) si les lignes sont strictement croissantes)*.

### 3. Tableaux des temps d'exécution (`sousMat1` vs `sousMat2`)

#### Variation de *n, m* (*n = m*) à *n', m'* (*k*) fixé (*k* = 10 et *k* = 50)

| *n* | *k* | Position `(i₀, j₀)` | `sousMat1` (ms) | `sousMat2` (ms) | Gain (`T₁ / T₂`) |
| :---: | :---: | :---: | :---: | :---: | :---: |
| 100 | 10 | `(90, 90)` | `[À compléter]` | `[À compléter]` | `[À compléter]` |
| 200 | 10 | `(190, 190)` | `[À compléter]` | `[À compléter]` | `[À compléter]` |
| 400 | 10 | `(390, 390)` | `[À compléter]` | `[À compléter]` | `[À compléter]` |
| 600 | 10 | `(590, 590)` | `[À compléter]` | `[À compléter]` | `[À compléter]` |
| 800 | 10 | `(790, 790)` | `[À compléter]` | `[À compléter]` | `[À compléter]` |
| 1000 | 10 | `(990, 990)` | `[À compléter]` | `[À compléter]` | `[À compléter]` |
| 100 | 50 | `(50, 50)` | `[À compléter]` | `[À compléter]` | `[À compléter]` |
| 200 | 50 | `(150, 150)` | `[À compléter]` | `[À compléter]` | `[À compléter]` |
| 400 | 50 | `(350, 350)` | `[À compléter]` | `[À compléter]` | `[À compléter]` |
| 600 | 50 | `(550, 550)` | `[À compléter]` | `[À compléter]` | `[À compléter]` |
| 800 | 50 | `(750, 750)` | `[À compléter]` | `[À compléter]` | `[À compléter]` |
| 1000 | 50 | `(950, 950)` | `[À compléter]` | `[À compléter]` | `[À compléter]` |

#### Variation de *n', m'* (*k*) à *n, m* fixé (*n = 1000*)

| *n* | *k* | Position `(i₀, j₀)` | `sousMat1` (ms) | `sousMat2` (ms) | Gain (`T₁ / T₂`) |
| :---: | :---: | :---: | :---: | :---: | :---: |
| 1000 | 10 | `(990, 990)` | `[À compléter]` | `[À compléter]` | `[À compléter]` |
| 1000 | 20 | `(980, 980)` | `[À compléter]` | `[À compléter]` | `[À compléter]` |
| 1000 | 30 | `(970, 970)` | `[À compléter]` | `[À compléter]` | `[À compléter]` |
| 1000 | 40 | `(960, 960)` | `[À compléter]` | `[À compléter]` | `[À compléter]` |
| 1000 | 50 | `(950, 950)` | `[À compléter]` | `[À compléter]` | `[À compléter]` |

### 4. Graphes comparatifs de `sousMat1` et `sousMat2`

> `[Insérer ici le graphe 1 : sousMat1 vs sousMat2 en fonction de n]`

> `[Insérer ici le graphe 2 : sousMat1 vs sousMat2 en fonction de k à n fixé]`

`[À compléter après tests : commenter l'accélération de sousMat2 grâce à la dichotomie O(log₂ m) et à l'élagage O(1) par rapport au balayage quadratique de sousMat1.]`
