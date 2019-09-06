# MCES in English

This code is for similarity calculation with MCES.

To run this code properly, you first need :

- To download Chebi Database CHebi_Lite.pdf https://www.dropbox.com/s/7k0ef9rmxvao70q/ChEBI_lite.sdf?dl=0 et save it in the folder MCES.

- You need to create folder "resultat" in the same branch as MCES.

- this code use SCIP, a solver of mathematical programs, especially linear programs. To use it on Linux, you have to:
    + Download SCIP Optimization suite (http://scip.zib.de/#download)
    + Unpack the archive
    + Move in the folder scipoptsuite-X.X.X where X.X.X is the download version of SCIP 
    + Install all the ddependencies : libpopt-dev, libncurses5-dev, libreadline-dev, libreadline6-dev, libtinfo-dev, zlib1g-dev, libgmp3-dev, libgmp-dev on debian or ubuntu
    + Run the command:  make scipoptlib
    + Move in the folder MCES
    + Add two symbolic links in the folder helpers 
            === ln -s LinkToScipoptsuite-X.X.X/scip-X.X.X/src/ scip
            === ln -s LinkToScipoptsuite-X.X.X/lib/libscipopt.a sciplib.a
            
            
            
Each molecule unique identifier is ChEBI number. For example : Water is 15377

To run the program: ./mesure_similarite MOL1 MOL2 type date size


    - MOL 1 : chebi id of the first molecule
    - MOL 2 : chebi id of the second molecule. If MOL2 = 0, then the program will compute the similarity of MOL1 with all the 90130 of Chebi Database
    - Type : 1 OR 2 
            * Type = 1 for similarity on molecular graphs
            * Type = 2 for similarity on modified molecular graphs (succesive suppression of the vertices of degree 1). It is the version used on the paper to compare to Graph of cycles.
    - Date : Time limit in seconds to calculate the similarity between two molecules (0 if we do not want time limit).
    - size : Limit size of the product graph to find the MCES (0 if we do not want size limit)
    
Example: ./mesure_similarite 15377 29412 2 20 600 : compute the similarity between chebi id 15377 and 29412 with a time limit of 20 seconds and a size max of 600 for the product graph.
        ./mesure_similarite 15377 2 0 0 0 :  compute the similarity between chebi id 15377 and all the molecules in Chebi Database with no fixed time limit or size max.
        
        
When MOL2 = 0, the result of the similarity is in the file ../resultats/similarite_mol1_all.data


HOW TO READ THE FILE OF RESULTS : 

On each line there are 2 integers and 1 float ( MOL1 MOL2 similarity) :

                      similarity = -1 : time limit exceed
                      similarity = -2 : size max exceed
                      similarity in [0 .. 1] : Exact value of similarity between MOL1 and MOL2. When the similariy is closed to 1.0, then the molecules are similar ccording to MCES.
                      

UP TO DATE.




# MCES in French
Calcule le degré de similarité avec l'algorithme MCES

Contraintes :

- La base de données de molecules utilisée est CHEBI, il faut télécharger le fichier CHebi_Lite.pdf(https://www.dropbox.com/s/7k0ef9rmxvao70q/ChEBI_lite.sdf?dl=0) et le mettre dans le dossier.

- faut créer un dossier : resultat à coté du dossier contenant le code, ce dossier contiendra les résultats de similarité.

- Contient un programme pour travailler avec SCIP, solveur de programmes mathématiques, en particulier de programmes linéaires. Pour l'utiliser sur Linux, il faut:
== Télécharger SCIP Optimization suite (http://scip.zib.de/#download) dans le dossier de votre choix
== Décompresser l'archive
== Aller dans le dossier scipoptsuite-X.X.X où X.X.X est la version de SCIP installée
== Exécuter la commande make scipoptlib après avoir installé les dépendances (libpopt-dev, libncurses5-dev, libreadline-dev, libreadline6-dev, libtinfo-dev, zlib1g-dev, libgmp3-dev, libgmp-dev sur debian ou ubuntu)
== Revenir dans le dossier MCES
== Rajouter dans le dossier helpers deux liens symboliques:
=== ln -s CheminVersScipoptsuite-X.X.X/scip-X.X.X/src/ scip
=== ln -s CheminVersScipoptsuite-X.X.X/lib/libscipopt.a sciplib.a

//fin des contraintes

On appele une molécule par son CHEBI ID.

La commande ./mesure_similarite mol1 mol2 type temps taille: 

                      mol1: chebi id de la première molecule
                      mol2: chebi id de la deuxième molécule( et 0 si on veut calculer la similarité de la molecule 1 avec toutes les autres molécules de la base de données.
                      type : 1 si le calcul de similarité sur la molécule entière et 2 si le calcul se fait sur la molecule sans branche( àpres élagage sucsésif des feuilles).
                      Temps: Temps limite en secondes pour calculer la similarité entre deux molecules (0 si on ne veut pas de limite en temps)
                      taille: Taille limite du graphe produit pour trouver le MCES.(0 si on ne veut pas de limite en taille)
                      
                      
Exemple: ./mesure_similarite 15377 29412 2 20 600 : calcule la similarité entre la molécule de chebi id 15377 et la molécule 29412 avec un temps limite de 20 secondes et de taille 600 max pour le graphe produit.
        ./mesure_similarite 15377 2 0 0 0 : calcule la similarité entre la molécule de chebi id 15377 et toutes les autres molécules de la base de données sans contrainte de temps ni de taille
        
        
Dans le cas où mol2 = 0, le résultat de la similarité se trouve dans le fichier ../resultats/similarite_mol1_all.data

Explications des résultats: 

Sur chaque ligne : mol1 mol2 similarite :

                      similarité = -1 : temps max de calcul atteint
                      similarité = -2 : taille max du graphe produit des deux molécules
                      similarité in [0 .. 1] : valeur de similarité entre mol1 et mol2. Plus similarité tend vers 1 plus les molécules sont similaires.
                      
                      
 Le code pour générer le pdf(présentant proprement les résultats avec images) ne se trouve pas dans ce dossier.
                    

A JOUR
