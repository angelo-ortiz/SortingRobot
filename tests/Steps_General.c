#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/lib/Grille.h"
#include "../include/lib/Solution.h"
#include "../include/src/AuPlusProche.h"
#include "../include/src/UneCaseParCouleur.h"
#include "../include/lib/API_AffGrille.h"

#define NB_ALGO (NB_GRAPHE + 1)
#define ITER 10
#define DEB 10
#define PAS 10

extern sr_algorithme gen_algorithmes[NB_ALGO];

void test_algorithme(FILE *f_algo, sr_algorithme algorithme, Grille *G, Solution *S, int n)
{
  int graine, pas_exe = 0;
  double pas_moy;
  for (graine = 0; graine < ITER; ++graine) {
    Gene_Grille(G,graine);
    Solution_init(S);
    algorithme(G, S, graine);
    pas_exe += S->cptr_pas;
    Solution_desallocation(S);
  }
  pas_moy = ((double) pas_exe) / ((double) graine);
  fprintf(f_algo, "%d %7.6e\n", n, pas_moy);
}

int main (int argc, char* *argv)
{
  int n, i;
  Grille G;
  Solution S;
  char *res_algo[NB_ALGO] = {"../data/general/avl.txt", "../data/general/g_naif.txt",\
			     "../data/general/g_ameliore.txt", \
			     "../data/general/g_general.txt", \
			     "../data/general/g_coupes.txt"};
  FILE *f_algo[NB_ALGO];
  int taille_max = 0;
  
  if (argc != 2) {
    printf("usage: %s <taille_grille_max>\n",argv[0]);
    return 1;
  }
  sscanf(argv[1], " %d", &taille_max);

  for (i = 0; i < NB_ALGO; ++i) {
    f_algo[i] = fopen(res_algo[i], "w");
    if (f_algo[i] == NULL) {
      exit(1);
    }
  }
  
  n = DEB;
  do {
    G.n = n;
    G.m = n;
    G.nbcoul = n;
    Grille_allocation(&G);
    
    for (i = 0; i < NB_ALGO; ++i) {
      test_algorithme(f_algo[i], gen_algorithmes[i], &G, &S, n);
    }
    printf("%d %d\n", taille_max, n);

    n += PAS;
    Grille_desallocation(&G);
  } while (n <= taille_max);
  
  for (i = 0; i < NB_ALGO; ++i) {
    fclose(f_algo[i]);
  }
  return 0;
}
