#include "helpers/proglin_helper.h"
#include "graph.h"
#include <stdlib.h>
#include <stdio.h>
/**
* Type concret représentant un graphe
*/

struct graph_s{
  int n; /* Nombre de noeuds */
  int** links; /* Matrice d'adjacence */
};

graph build_graph_from_matrix(int n, int** links){
  graph g = (graph) malloc(sizeof(struct graph_s));
  g->n = n;
  
  /* Attention à ne pas réécrire la matrice */
  g->links = links;
  
  /* Version safe
  int** linkscp = (int**) malloc(sizeof(int*) * n);
  int i,j;
  for(i = 0; i < n; i++){
    linkscp[i] = (int*) malloc(sizeof(int) * n);
    for(j = 0; j < n; j++){
      linkscp[i][j] = links[i][j];
    }
  }
  g->links = linkscp;
  */
  return g;
}

int nbnodes(graph g){
  return g->n;
}

int** build_matrix_from_graph(graph g){

  return g->links;
   
  /* Version safe
  int n = nbnodes(g);
 
  int** linkscp = (int**) malloc(sizeof(int*) * n);
  int i,j;
  for(i = 0; i < n; i++){
    linkscp[i] = (int*) malloc(sizeof(int) * n);
    for(j = 0; j < n; j++){
      linkscp[i][j] = g->links[i][j];
    }
  }
  return linkscp;
  */
}

void destroy(graph g){
  int n = nbnodes(g);

  /* Si links a été créé lors de la création du graphe
  int i;
  for(i = 0; i < n; i++){
    free(g->links[i]);
  } 
  free(g->links);
  */
  free(g);
}

int are_neighbors(graph g, int u, int v){
  return g->links[u][v];
}

int* maxclique(graph g, long timeout){
  /* Clique calculée avec un programme linéaire */

  
  /* Créer le programme  linéaire, n variables, une par noeud */ 

  int n = nbnodes(g);

  abpl pl = create_lp(n);
  
  int i,j;


  for(i = 0; i < n; i++){
    char name[12];
    sprintf(name, "x%d", i); /* en espérant qu'il y ait moins de 10^11 noeuds dans le graphe */
    add_variable_binary(pl, name);
  }

  /* Contraintes, une par non-arête : si deux noeuds u et v ne sont pas reliés, alors xu + xv <= 1 */


  for(i = 0; i < n; i++){
    for(j = i+1; j < n; j++){
      if(are_neighbors(g, i, j))
        continue;
      int colno[2] = {i, j};
      double row[2] = {1, 1};
      add_le_constraint(pl, 2, colno, row, 1); /* xi + xj <= 1*/
    }
  }



  /* Fonction objectif : maximiser le nombre de noeuds */

  int* colnoobj = (int*) malloc(sizeof(int)*n);
  double* rowobj = (double*) malloc(sizeof(double)*n);
  for(i = 0; i < n; i++){
    colnoobj[i] = i;
    rowobj[i] = 1;
  }

  set_maximization(pl);

  set_objective(pl, n, colnoobj, rowobj); /* max sum(x_u, u noeud de g) */

    

  /* Quantité de débug affiché à l'écran, ici on affiche rien */

  set_silent_verbose(pl);
  /* on indique le timeout en seconde */
  set_max_timeout(pl, timeout);

  /* Résolution, ret est le code de retour, qui explique comment s'est terminé la résolution */

  int ret = solve_pl(pl);


  int* clique = (int*) malloc(sizeof(int)*n);

  /* Si ret indique qu'on a trouvé une solution optimale */
  if(ret == ABLP_OPTIMAL || ret == ABLP_SUBOPTIMAL){
    for(i = 0; i < n; i++){
      double x = get_variable_value(pl, i);
      if(x > 0.5)
        clique[i] = 1;
      else
        clique[i] = 0;
    }
  }
  else{
    for(i = 0; i < n; i++)
      clique[i] = -1;
  }

  /* Destruction du programme linéaire */

  destroy_lp(pl);

  return clique;

}

