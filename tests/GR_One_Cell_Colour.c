#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/lib/Grille.h"
#include "../include/lib/Solution.h"
#include "../include/src/AuPlusProche.h"
#include "../include/src/UneCaseParCouleur.h"
#include "../include/lib/API_AffGrille.h"

#define ITER 10
#define DEB 10
#define PAS 10

extern sr_algorithme ucpc_algorithmes[NB_GRAPHE+2];

//TODO l'objectif est compter le nombre de pas => S != NULL !!!
double test_algorithme(FILE *f_algo, sr_algorithme algorithme, Grille *G, Solution *S, int n)
{
  int graine;
  clock_t temps_initial, temps_final;
  double temps_exe = 0., temps_moy;
  for (graine = 0; graine < ITER; ++graine) {
    Gene_Grille(G,graine);
    //Solution_init(S);
    temps_initial = clock();
    algorithme(G, S, graine);
    temps_final = clock();
    temps_exe += ((double) (temps_final - temps_initial))/((double) CLOCKS_PER_SEC);
    //Solution_desallocation(S);
  }
  temps_moy = temps_exe/((double) graine);
  fprintf(f_algo, "%d %7.6e\n", n, temps_moy);
  return temps_moy;
}

int main (int argc, char* *argv)
{
  int n, i;
  Grille G;
  //Solution S = NULL;
  char *res_algo[NB_GRAPHE + 2] = {"../data/vector/avl.txt", "../data/vector/ucpc.txt", \
				   "../data/vector/g_naif.txt",	"../data/vector/g_ameliore.txt", \
				   "../data/vector/g_general.txt", "../data/vector/g_coupes.txt"};
  FILE *f_algo[NB_GRAPHE + 2];
  int taille_max = 0;
  
  if (argc != 2) {
    printf("usage: %s <taille_grille_max>\n",argv[0]);
    return 1;
  }
  sscanf(argv[1], " %d", &taille_max);

  for (i = 0; i < NB_GRAPHE + 2; ++i) {
    f_algo[i] = fopen(res_algo[i], "w");
    if (f_algo[i] == NULL) {
      exit(1);
    }
  }
  
  n = DEB;
  do {
    G.n = n;
    G.m = 1;
    G.nbcoul = n;
    Grille_allocation(&G);
    
    for (i = 0; i < NB_GRAPHE + 2; ++i) {
      test_algorithme(f_algo[i], graphe_algorithmes[i], &G, NULL, n);
    }
    printf("%d %d\n", taill_max, n);

    n += PAS;
    Grille_desallocation(&G);
  } while (n <= taille_max);
  
  return 0;
}
