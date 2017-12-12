#include "fonctions_mces.h"
#include <string.h>

void free_memoire(struct molecule m)
{
	if ( m.liste_atomes != NULL)
		free(m.liste_atomes);
	if (m.liste_liaisons != NULL)
		free(m.liste_liaisons);

}
struct molecule suppression_aretes(struct molecule m, int i)
{
	struct molecule M;
	M.nb_liaisons = m.nb_liaisons - 1;
	M.nb_atomes = m.nb_atomes - 1;
	M.liste_atomes   = malloc(M.nb_atomes   * sizeof(int));
	M.liste_liaisons = malloc(M.nb_liaisons * sizeof(struct liaison));
	M.matrice_liaisons = NULL;
	M.nb_hydrogene=0;
	M.chebi_id = m.chebi_id;
	int j;
	for(j=0;j<1024;j++)  M.chebi_name[j] = m.chebi_name[j];


	int a,k = 0; 
	//mise a jour des atomes
	for (a = 0; a < m.nb_atomes ; a++) 
	{
		
		if(i -1 != a )
		{
			M.liste_atomes[k] = m.liste_atomes[a];
			k++;
		}
	}	

	k = 0;
	for (a = 0; a < m.nb_liaisons; a++) 
	{
		if(m.liste_liaisons[a].A1 != i && m.liste_liaisons[a].A2 != i )
		{
			if(m.liste_liaisons[a].A1 > i)
				M.liste_liaisons[k].A1 = m.liste_liaisons[a].A1 -1;
			else 
				M.liste_liaisons[k].A1 = m.liste_liaisons[a].A1;
			if(m.liste_liaisons[a].A2 > i )
				M.liste_liaisons[k].A2 = m.liste_liaisons[a].A2 -1 ;
			else
				M.liste_liaisons[k].A2 = m.liste_liaisons[a].A2 ;
			M.liste_liaisons[k].l_type = m.liste_liaisons[a].l_type;
			//fprintf(stdout, "%d %d %d %d\n",k,M.liste_liaisons[k].A1,M.liste_liaisons[k].A2, M.liste_liaisons[k].l_type);
		
			k++;
		}
	}	
	//mise a jour des liaisons
	
	for (a = 0 ; a < M.nb_atomes ; a++) 
	{
		if(M.liste_atomes[a] == 1) 
			M.nb_hydrogene++;
		
	}
	free_memoire(m);
	return M;
}
int verif(struct molecule m)
{
	int i ,verif = -1; 

	for ( i = 0; i < m.nb_atomes; i++)
	{
		if (degre[i] == 1)
		{
			verif = i;
			break;
		}
			
	}
	return verif;
}

struct molecule modification_structure(struct molecule m)
{
	int i = verif(m) ; 
	//printf("%d val de i\n",i );
	while(i != -1)
	{
		//fprintf(stdout, "le sommet %d\n",i +1  );
		m = suppression_aretes( m, i + 1);
		if ( degre != NULL)
			free(degre);
		degre = calcul_degre(m);
		//affichage_degre(m,degre);
		//affiche_mol(m);
		i = verif(m) ; 
	}
	return m;
}
int *calcul_degre( struct molecule m)

{	
	
	degre = malloc( m.nb_atomes * sizeof( int)); // le degré de tous les sommets du graphe
	if ( degre == NULL)
	{
		fprintf(stdout, "Cannot allocate memory to degre \n");
		exit(78);
	}

	int i; 
	for(i = 0; i < m.nb_atomes; i++)
		degre[i] = 0;
	//remplissage
	for ( i = 0; i < m.nb_liaisons; i++)
	{
		degre[m.liste_liaisons[i].A1 -1]++;
		degre[m.liste_liaisons[i].A2 -1]++;
	}

	return degre;
}

void affichage_degre(struct molecule m)
{
	fprintf(stdout, "Debut affichage liste des degrés\n");
	int i;
	for( i = 0; i < m.nb_atomes; i++)
		printf("%d %d\n",i+1, degre[i]);
	fprintf(stdout, "fin affichage liste des degrés\n" );
}
struct molecule elimination_feuilles(struct molecule m)
{
	degre = calcul_degre(m);
	//affichage_degre(m,degre);

	m = modification_structure(m);
	if(degre != NULL)
		free(degre);
	return m;
}
/*int main(int argc, char const *argv[])
{
	if( argc != 2)
	{
		fprintf(stdout, "Missing parameter : [numéro CHEBI de la molécule ]\n");
		exit(1);
	}
	int mol_courante = atoi(argv[1]);

	char dossier[128];
	sprintf(dossier,"../CHEBI/%d/%d.sdf",mol_courante,mol_courante);
	FILE *F = fopen(dossier,"r");

	if( F == NULL)
	{
		fprintf(stdout, "Cannot open the file %s \n",dossier );
		exit(2);

	}
	init_atom_num();
	struct molecule m = lire_molecule(F);
	//fprintf(stdout,"%d %d %d \n",m.chebi_id , m.nb_atomes, m.nb_liaisons);
	//affiche_mol(m);
	m = elimination_feuilles(m);
	//fprintf(stdout,"%d %d %d \n",m.chebi_id , m.nb_atomes, m.nb_liaisons);
	//affiche_mol(m);
	free_memoire(m);
	fclose(F);
	exit(0);
}*/
