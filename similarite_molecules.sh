#!/bin/sh

#  similarite_molecules.sh
#  
#
#  Created by Stefi Nouleho on 26/07/2017.
#

# calcul de la similarite des molecules se trouvant dans le fichier molecules.txt

fichiermol="molecules.txt"
while read chebiid temps taille; do

    fichier="resultats/similarite_"$chebiid"_"$temps"_"$taille"_all.data"
    if [ ! -f "$fichier" ]; then
        echo "$chebiid $temps $taille"
        ./mesure_similarite $chebiid 0 1 $temps $taille
    fi
done <"$fichiermol"
