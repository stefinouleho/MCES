# MCES
Calcule le degré de similarité avec l'algorithme MCES

Contraintes :

- La base de données de molecules utilisée est CHEBI, il faut télécharger le fichier CHebi_Lite.pdf(https://www.dropbox.com/s/4qqrg5jxi8o727w/ChEBI_lite.sdf?dl=0) et le mettre dans le dossier.

-faut créer un dossier : resultat à coté du dossier contenant le code, ce dossier contiendra les résultats de similarité.


//fin des contraintes

On appele une molécule par son CHEBI ID.

La commande ./mesure_similarite mol1 mol2 temps taille: 

                      mol1: chebi id de la première molecule
                      mol2: chebi id de la deuxième molécule( et 0 si on veut calculer la similarité de la molecule 1 avec toutes les autres molécules de la base de données.
                      Temps: Temps limite en secondes pour calculer la similarité entre deux molecules (0 si on ne veut pas de limite en temps)
                      taille: Taille limite du graphe produit pour trouver le MCES.(0 si on ne veut pas de limite en taille)
                      
                      
Exemple: ./mesure_similarite 15377 29412 20 600 : calcule la similarité entre la molécule de chebi id 15377 et la molécule 29412 avec un temps limite de 20 secondes et de taille 600 max pour le graphe produit.
        ./mesure_similarite 15377 0 0 0 : calcule la similarité entre la molécule de chebi id 15377 et toutes les autres molécules de la base de données sans contrainte de temps ni de taille
        
        
Dans le cas où mol2 = 0, le résultat de la similarité se trouve dans le fichier ../resultats/similarite_mol1_all.data

Explications des résultats: 

Sur chaque ligne : mol1 mol2 similarite :

                      similarité = -1 : temps max de calcul atteint
                      similarité = -2 : taille max du graphe produit des deux molécules
                      similarité in [0 .. 1] : valeur de similarité entre mol1 et mol2. Plus similarité tend vers 1 plus les molécules sont similaires.
                      
                      
 Le code pour générer le pdf(présentant proprement les résultats avec images) ne se trouve pas dans ce dossier.
                    

