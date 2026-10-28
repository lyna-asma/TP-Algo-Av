### TP1 notes :

- ` Random100.txt` , 100 nombres premiers au hasard

- `Random1000.txt` , 1000 nombres premiers.

- `Test-1.txt` , 100 nombres premiers choisis uniquement entre 1000 et 1000000.

- ` Test-2.txt` , ici c est des nombres classses par taille , 
10 nombres à 3 chiffres (ex: 103, 127...),
10 nombres à 4 chiffres (ex: 1259...) ,
... jusqu'à 10 nombres géants à 12 chiffres !

- ` Test-3.txt` , 40 nombres seulement, mais très espacés : 10 à 3 chiffres, 10 à 6 chiffres, 10 à 9 chiffres et 10 à 12 chiffres.

### Comande a excuter :
``` tp1.exe Test-3.txt > resultats_Test-3.csv ```

### remarques :
- Sur les petits nombres (3 chiffres) : 
Tous les algorithmes sont instantanés, on ne voit presque pas de différence.
- Sur les nombres moyens (6 chiffres) : L'algorithme 1 commence à être 1000 fois plus lent que l'algorithme 5.
- Sur les nombres géants (12 chiffres) :
L'algorithme 1 mettrait des heures à répondre (c'est pour ça qu'on a mis -1, pour ne pas faire planter la machine).
L'algorithme 5 qui utilise la racine carrée répond en 0.0002 seconde !

``` /* ---------- Algorithme A5 : i de 2 a sqrt(n) ---------- */
int A5(ll n) {
    int premier = 1;
    ll i = 2;
    ll r = (ll) sqrt((double) n);   /* calcule une seule fois, hors de la boucle */
    while (i <= r && premier == 1) {
        if (n % i == 0) premier = 0;
        else i = i + 1;
    }
    return premier;
}

/* ---------- Algorithme A6 : n pair elimine, puis i impair de 3 a sqrt(n) ---------- */
int A6(ll n) {
    int premier;
    ll i;
    if (n != 2) {
        if (n % 2 == 0) premier = 0;
        else {
            premier = 1;
            i = 3;
            ll r = (ll) sqrt((double) n);
            while (i <= r && premier == 1) {
                if (n % i == 0) premier = 0;
                else i = i + 2;
            }
        }
    } else premier = 1;
    return premier;
}```