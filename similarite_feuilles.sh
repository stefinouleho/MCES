#!/bin/sh

#  similarite_molecules.sh
#  
#
#  Created by Stefi Nouleho on 12/10/2017.
#

# calcul de la similarite des molecules se trouvant dans le fichier molecules.txt apres epluchage des feuilles 

fichiermol="molecules.txt"
while read chebiid temps taille; do

    fichier="resultats/similarite_mces"$chebiid"_"$temps"_"$taille"_all.data"
    if [ ! -f "$fichier" ]; then
        echo "$chebiid $temps $taille"
        ./mesure_similarite $chebiid 0 2 $temps $taille
    fi
done <"$fichiermol"