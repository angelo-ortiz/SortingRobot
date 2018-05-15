#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/lib/Grille.h"
#include "../include/lib/Solution.h"
#include "../include/src/AuPlusProche.h"
#include "../include/lib/API_AffGrille.h"

#define NB_ALGO 2
#define ITER 10
#define DEB 5
#define PAS 5

sr_algorithme algorithmes_couleurs[NB_ALGO] = {algorithme_parcouleur, algorithme_paravl};

void test_algorithme(FILE *f_algo, sr_algorithme algorithme, Grille *G, Solution *S, int nbcoul)
{
  int graine;
  clock_t temps_initial, temps_final;
  double temps_exe = 0., temps_moy;
  for (graine = 0; graine < ITER; ++graine) {
    Gene_Grille(G, graine);
    //Solution_init(S);
    temps_initial = clock();
    algorithme(G, S, graine);
    temps_final = clock();
    temps_exe += ((double) (temps_final - temps_initial))/((double) CLOCKS_PER_SEC);
    //Solution_desallocation(S);
  }
  temps_moy = temps_exe/((double) graine);
  fprintf(f_algo, "%d %7.6e\n", nbcoul, temps_moy);
}

int main (int argc, char* *argv)
{
  int n, nbcoul, i;
  Grille G;
  //Solution S = NULL;
  char *res_algo[NB_ALGO] = {"../data/colour/couleur.txt","../data/colour/avl.txt"};
  FILE *f_algo[NB_ALGO];

  if (argc != 2) {
    fprintf(stderr, "usage: %s <taille_grille>\n", argv[0]);
    return 1;
  }
  
  n = atoi(argv[1]);
  if (n < DEB) {
    fprintf(stderr, "La taille de la grille doite etre superieure ou egale a %d\n", DEB);
    return 1;
  }
  
  for (i = 0; i < NB_ALGO; ++i) {
    f_algo[i] = fopen(res_algo[i], "w");
    if (f_algo[i] == NULL) {
      exit(1);
    }
  }

  G.m = n;
  G.n = n;
  nbcoul = DEB;
  do {
    G.nbcoul = nbcoul;
    Grille_allocation(&G);

    for (i = 0; i < NB_ALGO; ++i) {
      test_algorithme(f_algo[i], algorithmes_couleurs[i], &G, NULL, nbcoul);
    }
    printf("%d %d\n", n, nbcoul);

    nbcoul += PAS;
    Grille_desallocation(&G);
  } while (nbcoul <= n);

  for (i = 0; i < NB_ALGO; ++i) {
    fclose(f_algo[i]);
  }
  return 0;
}
