#include "fonctions_mces.h"

double last_chrono;
int main(int argc, char *argv[])
{
	
	
	if( argc != 6){
		fprintf(stdout,"Missing arguments (num chebi1 , num chebi 2 ( 0  for all) ,type, date , lenght limit )\n");
		exit(20);
	}

	//Read molecules in the fichier chebi_lite.sdf
	struct molecule *M = lecture_fichier_chebi();
	
	//recuperation des parametres
	int num_chebi1, num_chebi2,taille_limite,type;
	float date;
	num_chebi1 = atoi(argv[1]);
	num_chebi2 = atoi(argv[2]);
	type 	= atoi(argv[3]);// type = 1 calcul sur le graphe moléculaire et type = 2 calcul sur le graphe après elagage succesif des feuilles.
	date 	   = atof(argv[4]); //temps max de calcul en secondes
	taille_limite = atoi(argv[5]); // la taille max du graphe produit
	
	fprintf(stdout," donnees recus :\n molecule de chebi 1 = %d et chebi 2 = %d  avec type = %d , la date limite de  %f s et taille limite de %d sommets\n" , num_chebi1,num_chebi2,type, date,taille_limite);
	
	float r;
	int pos1,pos2;
	if( num_chebi2 != 0)
	{
		last_chrono = chrono();
		if( type  == 1)
		{
			r = mesure_similarite( num_chebi1, num_chebi2,M, date, taille_limite);
			fprintf(stdout,"similarite mesure entre %d et %d est de : %f\n",num_chebi1,num_chebi2,r);
		}
		else if( type == 2 )
		{
			pos1 = position_M(num_chebi1,M);
			pos2 = position_M(num_chebi2,M);
			M[pos1]= elimination_feuilles(M[pos1]);
			M[pos2]= elimination_feuilles(M[pos2]);
			r = mesure_similarite( num_chebi1, num_chebi2,M, date, taille_limite);
			fprintf(stdout,"similarite mesure entre %d et %d est de : %f\n",num_chebi1,num_chebi2,r);
		}
		else{
			fprintf(stdout," valeur du type incorrect,type = 1 calcul sur le graphe moleculaire et type = 2 calcul sur le graphe apres elagage succesif des feuilles.\n");
		}
	}
	
	else
	{
		similarite_all(num_chebi1, M,type, date, taille_limite);
		
		
	}	
	
	int nb_mol;
	printf("3. Libération de la mémoire : %.3lf s\n",chrono());

	for(nb_mol=0 ; nb_mol < NB_MOLECULES ; nb_mol++) 
		liberer_molecule(M[nb_mol]);
	free(M);

	exit(0);
}
