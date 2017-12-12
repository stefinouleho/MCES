#include "lecture_molecule_sdf.h"
#include "helpers/graph.h"


struct couple{
	int a1;
	int a2;
}couple;

double chrono();
int position_M( int g1_chebi,struct molecule *M);
struct molecule construction_matrice_mol(struct molecule m);
void affiche_matrice(struct molecule m);
struct couple *construction_couples(struct molecule *M,int pos1, int pos2,int taille);
graph graphe_produit(int g1_chebi,int g2_chebi,struct molecule *M);
void calcul_cl(graph m,int *dans_clique,int taille_clique,int *candidat,int taille_candidat, double date);	
void la_clique_max(graph m,double date);
int* graphe_g12(graph g12, int* clique_max, struct molecule *M, int g1_chebi, int g2_chebi);
void liberer_molecule(struct molecule g);
struct molecule * lecture_fichier_chebi();
float mesure_similarite (int g1_chebi,int g2_chebi,struct molecule *M,double date,int taille_limite);
void similarite_all(int g1_chebi,struct molecule *M,int type, double date,int taille_limite);
int nb_lignes( FILE *F);
//fonctions feuilles

int *degre;

void free_memoire(struct molecule m);
struct molecule modification_structure(struct molecule m);
struct molecule elimination_feuilles(struct molecule m);
int *calcul_degre( struct molecule m);
void affichage_degre(struct molecule m);
int verif(struct molecule m);
