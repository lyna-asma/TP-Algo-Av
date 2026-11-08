# A
## pour data :
generer des tableaux de donness 100 milles , 200 milees ..... avec la fct random pour  la creation des doneees (fichiers qui contiennemt)

## code 
3 fonction d eecherche

## tests
- mesure de temps d exec 
 - petite comparaison analyse et comparaison des resutas
 ![alt text](image-1.png)

 # B
## code 
- approche simple 
- approche plus efficace : comparaison par deux nombre 
tab : [2 5 3 7 1 8 4  ....]
1 - compr 5 2 et fair lapermut
2- comp le 3 et 7 est faire permut 
3 - puis on compare ces dernies entre eux deux a deux et permute 
donc : ....
4 - ajouter a chaque procedure des instructions 
comme cca on aura les val minimales ans les cases impaires et inversemnet
## tetss
 -le parametre est le nombre de comparaison de nombres
 -Quelle est sa complexité théorique au pire cas en nombre de 
comparaisons ? (rapport)
- tableau : 
N : de 100 m a 800 m
__|______________    min-max-a ___________________|____________min-ma-b____________|
N | tmp | nbcomparaisons (avec le conteur ajoute )|         temp | nb comp         |  
 ## donness

 tableaux qui contient de elements des donnees aleatoires de 100 miles element a 800 mille eleements 

### notes :
 - Les 10 tailles générées (100 000, 200 000, 400 000, 600 000, 800 000, 1 000 000, 1 200 000, 1 400 000, 1 600 000, 1 800 000)
- Partie A 100 000 à 800 000 (et au-delà) pour la Partie B.
- Travailler sur de grands volumes pour que les temps d'exécution dépassent le seuil de précision de l'horloge CPU
- La Partie B impose deux conditions simultanées :
« Nous supposons ici que les valeurs de l'ensemble considéré sont distinctes ».
« Remplir le tableau avec des valeurs aléatoires (fonction random) ».
``` random.sample``` plutôt que ``` random.randint ``` , car cette derniere fait un tirage avec remise et l autre non
- ``` random.seed(42)```le script génère exactement les mêmes séquences de nombres à chaque exécution et sur toutes les machines du groupe,

## Rapport


