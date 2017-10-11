#include "lecture_molecule_sdf.h"

int *degre;

void free_memoire(struct molecule m);
struct molecule modification_structure(struct molecule m);
struct molecule elimination_feuilles(struct molecule m);
int *calcul_degre( struct molecule m);
void affichage_degre(struct molecule m);
int verif(struct molecule m);