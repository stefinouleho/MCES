# MCES
Calcule le degré de similarité avec l'algorithme MCES


La base de données de molecul utilisé est CHEBI, il faut téléchargr le fichier CHebi_Lite.pdf et le mettre dans le dossier.

On appele une molécule par son CHEBI ID.

La commande ./mesure_similarite mol1 mol2 temps taille: 

                      mol1: chebi id de la première molecule
                      mol2: chebi id de la deuxième molécule( et 0 si on veut calculer la similarité de la molecule 1 avec toutes les autres molécules de la base de données.
                      Temps: Temps limite en secondes pour calculer la similarité entre deux molecules (0 si on ne veut pas de limite en temps)
                      taille: Taille limite du graphe produit pour trouver le MCES.(0 si on ne veut pas de limite en taille)
                      
                      
Exemple: ./mesure_similarite 4672 15872 20 600 : calcule la similarité entre la molécule de chebi id 4672 et la molécule 15872 avec un temps limite de 20 secondes et de taille 600 max pour le graphe produit.
