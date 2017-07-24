#include "fonctions_mces.h"

#define AUCUNE_LIAISON (-1024)

int taille_clique_max = 0;
int *dans_clique_max= NULL;
int liaison_max = 0;
double last_chrono;

double chrono() 
{
	struct timeval tv;
	static double date_deb = 0.0;
	double date_courante;
	gettimeofday(&tv,NULL);
	date_courante = tv.tv_sec + ((double)tv.tv_usec)*1e-6;
	if (date_deb == 0.0) date_deb = date_courante;
	return date_courante-date_deb;
}

int position_M( int g1_chebi,struct molecule *M)
{ // trouve la poition d'une molecule de chebi g1_chebi dans M
	int i;
	
	for(i=0;i< NB_MOLECULES ;i++)
	{
		if(M[i].chebi_id == g1_chebi)
			return i;
	}
	if(i == NB_MOLECULES)
	{ 
		fprintf(stderr,"numero de chebi non present dans la base \n");
		exit(5); 
	}
	
	return 0;
}



struct molecule construction_matrice_mol(struct molecule m)
{//construction de la matrice de liaison d'une molecule
	
	int i,j;
	if(m.matrice_liaisons == NULL)
	{
		m.matrice_liaisons =  malloc(m.nb_atomes * sizeof(int *));
		
		for(i=0;i< m.nb_atomes;i++) m.matrice_liaisons[i] =  malloc(m.nb_atomes * sizeof(int));
		
		for(i=0;i< m.nb_atomes;i++)
		{
			for(j=0;j< m.nb_atomes;j++)
			 m.matrice_liaisons[i][j] = AUCUNE_LIAISON;
		}

		for(i =0; i< m.nb_liaisons;i++)
		{
			m.matrice_liaisons[m.liste_liaisons[i].A1-1][m.liste_liaisons[i].A2-1]= m.liste_liaisons[i].l_type;
			m.matrice_liaisons[m.liste_liaisons[i].A2-1][m.liste_liaisons[i].A1-1]= m.liste_liaisons[i].l_type;
		}
		

	}

	return m;
	
}


void affiche_matrice(struct molecule m)
{//affcihe la matrice d'une molecule m
	int i,j;
	printf("Affichage de la matrice m \n");
	
	if( m.matrice_liaisons == 	NULL)
	{
		printf("La matrice de cette molecule n'a pas encore eté défini \n");
		return;
	}
	for(i=0;i< m.nb_atomes;i++)
	{
		for(j=0;j< m.nb_atomes;j++)
			printf("%d ",m.matrice_liaisons[i][j] );
		
		printf("\n");
	}
	
}

struct couple *construction_couples(struct molecule *M,int pos1, int pos2,int taille)
{//Construction des couples d'atomes compatibles
	
	int n = 0,i,j;
	
	struct couple *couple_at;
	couple_at = malloc(taille * sizeof(couple));

	for(i= 0; i < M[pos1].nb_atomes;i++)
	{ 
		for(j= 0; j < M[pos2].nb_atomes;j++)
		{
			if( M[pos1].liste_atomes[i] == M[pos2].liste_atomes[j])
			{
				couple_at[n].a1 = i;
				couple_at[n].a2 = j;
				n++;
			}
		}
	}
	return couple_at;

}



struct molecule graphe_produit(int g1_chebi,int g2_chebi,struct molecule *M)
{ //prend en entrée les chebi id de deux molecules  et contruit le graphe produit
		
		
	struct molecule g12;
	g12.liste_liaisons = NULL;
	g12.matrice_liaisons = NULL;
	g12.liste_atomes = NULL;
	//trouve la position des molecules g1 et g2
	int taille= 0,pos1,pos2;
	pos1 =position_M(g1_chebi,M);
	pos2 =position_M(g2_chebi,M);
	
	//calcul de la taille du graphe produit
	int i,j;
	for(i= 0; i < M[pos1].nb_atomes; i++)
	{ 
		for(j= 0; j < M[pos2].nb_atomes;j++)
		{
			if( M[pos1].liste_atomes[i] == M[pos2].liste_atomes[j]) taille ++;
		}
	}
	
	//couple de liaisons entre les nouveaux sommets
	struct couple * couple_atome = construction_couples(M,pos1,pos2,taille);
	
	//construction de la matrice de liaison d'une molecule
	M[pos1]= construction_matrice_mol(M[pos1]);
	M[pos2]=construction_matrice_mol(M[pos2]);
	
	//initialisation de g12
	g12.nb_atomes = taille;
	g12.nb_liaisons = 0;
	if( g12.nb_atomes > 0)
	{
		g12.liste_atomes = malloc(g12.nb_atomes * sizeof(int));
		if(g12.liste_atomes == NULL)
		{
			fprintf(stdout, "Cannot allocate memory \n" );
			exit(33);
		}
	}
	for(i = 0;i < g12.nb_atomes;i++) 
		g12.liste_atomes[i] = M[pos1].liste_atomes[couple_atome[i].a1];	
	
	if( g12.nb_atomes > 0)
	{	
		g12.matrice_liaisons =  malloc(g12.nb_atomes * sizeof(int *));
		
		for(i = 0 ;i < g12.nb_atomes;i++) g12.matrice_liaisons[i] =  malloc(g12.nb_atomes * sizeof(int));
	}
		//remplissage des atomes	
	for(i = 0;i < g12.nb_atomes;i++) 
		g12.liste_atomes[i] = M[pos1].liste_atomes[couple_atome[i].a1];	
	//remplissage des liaisons
	int i1,i2,j1,j2;
	for(i= 0; i < taille ;i++)
	{
		for(j= 0; j < taille ;j++)
			g12.matrice_liaisons[i][j] = 0;
		
	}

	
	for(i= 0; i < taille ;i++)
		g12.matrice_liaisons[i][i] = 1;

	for(i= 0; i < taille ;i++)
	{ 
		i1 = couple_atome[i].a1;
		i2 = couple_atome[i].a2;
		for(j= i + 1 ; j < taille ;j++)
		{
			
			j1=couple_atome[j].a1;
			j2=couple_atome[j].a2;
			
			if( M[pos1].matrice_liaisons[i1][j1] == M[pos2].matrice_liaisons[i2][j2] )
			{

				if(((i1 == j1) && (M[pos2].matrice_liaisons[i2][j2] != AUCUNE_LIAISON)) || ((i2 == j2) && (M[pos1].matrice_liaisons[i1][j1] != AUCUNE_LIAISON)) ||( (i1 != j1) && (i2!=j2))||( (i1 ==j1) && (i2==j2)) )
				{
					g12.matrice_liaisons[i][j] = 1;
					g12.matrice_liaisons[j][i] = 1;
					
									
				}
			}
			
		}
	}
	free(couple_atome);
	//affiche_matrice(g12);
	return g12;
	
}

void liberer_molecule(struct molecule g) 
{ //liberation de l'espace memoire d'une molecule
	if (g.liste_atomes != NULL)
	{
		free(g.liste_atomes);
	}
	if (g.liste_liaisons != NULL ) free(g.liste_liaisons);
	if (g.matrice_liaisons)
	{	
		int i;
		for (i=0 ; i<g.nb_atomes ; i++) free(g.matrice_liaisons[i]);
		free(g.matrice_liaisons);
	}
} 
void calcul_cl(struct molecule m,int *dans_clique,int taille_clique,int *candidat,int taille_candidat, double date)
{	//calcul de la clique max recursif
	
	if (date != 0)
	{	
		if(chrono() - last_chrono > date) 
			return;
	}
	int i,j;
	if( taille_candidat == 0)
	{
		if(taille_clique > taille_clique_max){
			taille_clique_max = taille_clique;
			for (i = 0 ;  i < m.nb_atomes ; i ++)
				dans_clique_max[i] = dans_clique[i];
		}
		return;
	}
	
	if (taille_candidat + taille_clique <= taille_clique_max)
	{

		return;
	}
	
	//else 
	int taille_candidat_temp;
	int *candidat_temp;
	candidat_temp = malloc( m.nb_atomes * sizeof(int));



	for (i = 0 ;  i < m.nb_atomes ; i ++)
	{
		if ( candidat[i] == 1)
		{
			candidat[i] = 0;
			dans_clique[i] = 1 ;
			taille_candidat_temp = taille_candidat;
			
			for (j = 0 ;  j < m.nb_atomes ; j ++)
			{
				candidat_temp[j] = candidat[j]; 
				if ((candidat[j] == 1) && (m.matrice_liaisons[i][j] == 0))
				{
					candidat_temp[j] = 0;
					taille_candidat_temp--;	
				}	
			}
			
			taille_candidat_temp --;

			calcul_cl(m,dans_clique,taille_clique + 1,candidat_temp,taille_candidat_temp,date);
			dans_clique[i] = 0;
			
		}	
		
	}
	free(candidat_temp);
		 
}

	
void la_clique_max( struct molecule m,double date)
{	//Debut calcul de la clique -- Initialisation
	int i;
	int *candidat;
	int *dans_clique;
	
	dans_clique_max = malloc( m.nb_atomes *sizeof(int));
	if (!dans_clique_max) { fprintf(stderr,"cannot malloc dans_clique_max %d\n",m.nb_atomes); exit(41); }
	dans_clique = malloc( m.nb_atomes *sizeof(int));
	if (!dans_clique) { fprintf(stderr,"cannot malloc dans_clique %d\n",m.nb_atomes); exit(42); }
	candidat = malloc( m.nb_atomes *sizeof(int));
	if (!candidat) { fprintf(stderr,"cannot malloc candidat %d\n",m.nb_atomes); exit(43); }
	
	//initialisation 
	for(i = 0; i < m.nb_atomes ; i++ )
	{
		candidat[i] 	= 1;
		dans_clique[i]	= 0;
		dans_clique_max[i] = 0;
	}
	
	taille_clique_max = 0;
	
	calcul_cl(m,dans_clique,0,candidat,m.nb_atomes,date); // 0 taille de la clique initial  et m.nb_atome = nb sommets candidats
	 
	free(dans_clique);
	free(candidat);
	
}


struct molecule graphe_g12(struct molecule g12, struct molecule *M, int g1_chebi, int g2_chebi)
{ //contruction du graphe commun

	struct molecule clique;
	int nb_at =0,nb_liaisons=0,i,j,i1,j1;
	int pos1,pos2;
	pos1 =position_M(g1_chebi,M);
	pos2 =position_M(g2_chebi,M);
	
	struct couple *couple_atome = construction_couples(M,pos1,pos2,g12.nb_atomes);
	
	int tab[g12.nb_atomes];
	for(i=0;i < g12.nb_atomes ; i++)
	{
		tab[i] = dans_clique_max[i];
	}
	free(dans_clique_max);
	
	for(i=0;i < g12.nb_atomes - 1; i++)
	{
		if(tab[i] == 1)
		{
			for(j=i+1;j < g12.nb_atomes ; j++)
			{
				if(tab[j]==1 && (couple_atome[i].a1 == couple_atome[j].a1))
						tab[j] = 0;
			}
		}
	}

	for(i=0;i < g12.nb_atomes ; i++)
	{
		if(tab[i] ==1)
			nb_at++;
	}

	for(i=0;i < g12.nb_atomes - 1; i++)
	{
		if(tab[i] == 1)
		{
			for(j = i+1;j < g12.nb_atomes; j++)
			{
				if(tab[j] == 1)
				{
					
					i1 = couple_atome[i].a1;
					j1 = couple_atome[j].a1;
					if(M[pos1].matrice_liaisons[i1][j1] != AUCUNE_LIAISON)
						nb_liaisons ++;
				}
			}
		}
	}
	free(couple_atome);
	
	clique.nb_liaisons = nb_liaisons;
	clique.nb_atomes= nb_at;
	return clique;
	
}




struct molecule * lecture_fichier_chebi()
{//lecture du fichier chebi.sdf
	
	FILE *F;
	F = fopen("ChEBI_lite.sdf","r");
	
	if ( F == NULL ) 
	{
		 fprintf(stderr,"Cannot open ChEBI_lite.pdf file\n"); 
		 exit(1); 
	}
	init_atom_num();
	int nb_mol, DEB = 0, FIN = NB_MOLECULES;
	struct molecule *M = malloc(NB_MOLECULES*sizeof(struct molecule));
	
	if (M == NULL)
	{
		fprintf(stderr,"Not enough memory for M\n"); 
		exit(3); 
	}
	struct molecule m;
	printf("1. Lecture des molecules : %.3lf s\n",chrono());

	for(nb_mol = DEB ; nb_mol < FIN ; nb_mol++)
	{
		if (nb_mol % 1000 == 0) 
		{ 
			fprintf(stdout,"\r%5d / %d",nb_mol,FIN);
			fflush(stdout); 
		}
		m = lire_molecule(F);
		M[nb_mol] = m;
	}
	
	fclose(F);
	fprintf(stdout,"\r%5d / %d\n",nb_mol,FIN); 
	printf("Fin de la Lecture des molecules : %.3lf s\n",chrono());
	return M;
	
}

float mesure_similarite (int g1_chebi,int g2_chebi,struct molecule *M,double date,int taille_limite)
{//calcul du degré de similarité
	
	float similarite= -14;
	int pos1,pos2;
	pos1 = position_M(g1_chebi,M);
	pos2 = position_M(g2_chebi,M);
	
	struct molecule g12 = graphe_produit(g1_chebi,g2_chebi,M);
	
	if( taille_limite != 0 && ( g12.nb_atomes > taille_limite))
	{
		similarite = -2;
	}
	else
	{
		
		la_clique_max(g12,date);
		
		struct molecule clique= graphe_g12(g12,M,g1_chebi,g2_chebi);
		
		
		if(date == 0 || (chrono() - last_chrono <= date))
		{
			float num = (float)((clique.nb_atomes + clique.nb_liaisons)*(clique.nb_atomes + clique.nb_liaisons));
			float denum = (float)((M[pos1].nb_atomes + M[pos1].nb_liaisons)*(M[pos2].nb_atomes + M[pos2].nb_liaisons));
			similarite = num/denum;
		}
		else
		{
			similarite = -1;
		}
	}
	
	if(g12.liste_atomes != NULL)
		free(g12.liste_atomes);
	if(g12.liste_liaisons != NULL)
		free(g12.liste_liaisons);
	if(g12.matrice_liaisons != NULL)
	{
		int i;
		for (i = 0; i < g12.nb_atomes;i++)
			free(g12.matrice_liaisons[i]);
	}
	free(g12.matrice_liaisons);
	return similarite;
}

void similarite_all(int g1_chebi,struct molecule *M,double date,int taille_limite)
{
	int i;
	char nom[64];
	char nom2[64];
	sprintf(nom2,"%d",g1_chebi);
	strcpy(nom, "resultats/similatite_");
	strcat(nom,nom2);
	strcat(nom,"_all.data");
	FILE *F;
	F = fopen( nom, "w");
	if(F == NULL){
		fprintf(stdout,"Impossible de creer le fichier de resultat\n");
		exit(45);
	}
	
	float r;
	
	for ( i = 0;  i < NB_MOLECULES;  i++)
	{
		last_chrono = chrono();
		if (i % 1 == 0) 
		{ 
			fprintf(stdout,"\r%5d / %d (%3d atomes) %.3lf ",i,NB_MOLECULES,M[i].nb_atomes,last_chrono);
			fflush(stdout); 
		}
		r = mesure_similarite( g1_chebi, M[i].chebi_id,M, date, taille_limite);
		fprintf(F, "%d %f\n",M[i].chebi_id,r);
		fflush(F);
	}
	
	fclose(F);
}
