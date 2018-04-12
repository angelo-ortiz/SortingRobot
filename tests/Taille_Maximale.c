#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/lib/Grille.h"
#include "../include/lib/Solution.h"
#include "../include/src/AuPlusProche.h"
#include "../include/src/UneCaseParCouleur.h"
#include "../include/lib/API_AffGrille.h"

#define ITER 10
#define DEB 5

extern gen_algorithme gen_algorithmes[NB_APP + NB_GRAPHE];

double test_algorithme(FILE *f_algo, gen_algorithme algorithme, Grille *G, Solution *S, int n)
{
  int graine;
  clock_t temps_initial, temps_final;
  double temps_exe = 0., temps_moy;
  for (graine=0; graine < ITER; ++graine) {
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

int main (int argc, char** argv)
{
  int pas, n, i, sol = 1, cpt = 0;
  int fin_algo[NB_APP + NB_GRAPHE] = {0,0,0,0,0,0,0}, sortie_prog = 0;
  Grille G;
  //Solution S = NULL;
  int graine;
  char *nomfic;
  char *res_algo[NB_APP + NB_GRAPHE] = {"../data/naif.txt", "../data/circulaire.txt", \
					"../data/couleur.txt","../data/avl.txt", \
					"../data/g_naif.txt", "../data/g_ameliore.txt", \
					"../data/g_general.txt"};
  FILE *f_algo[NB_APP + NB_GRAPHE];
  clock_t temps_initial, temps_final;
  double temps_moy = 0., temps_max = 0.;

  if (argc != 2) {
    printf("usage: %s <temps_max>\n",argv[0]);
    return 1;
  }
  sscanf(argv[1], " %lf", &temps_max);

  pas = 5;
  
  for (i = 0; i < NB_APP + NB_GRAPHE; ++i) {
    f_algo[i] = fopen(res_algo[i], "w");
    if (f_algo[i] == NULL) {
      exit(1);
    }
  }
  
  n = DEB;
  do {
    G.n = n;
    G.m = n;
    G.nbcoul = n/2;
    Grille_allocation(&G);
    for (i = 0; i < NB_APP + NB_GRAPHE; ++i) {
      if (!fin_algo[i]) {
	temps_moy = test_algorithme(f_algo[i],gen_algorithmes[i], &G, NULL, n);
	if (temps_moy > temps_max) {
	  fin_algo[i] = 1;
	  sortie_prog += 1;
	  fclose(f_algo[i]);
	}
      }
    }
    cpt ++;
    Grille_desallocation(&G);
    if (sortie_prog >= 2 && cpt >= 20) {
      pas *= 2;
      cpt = 0;
    }
    fprintf(stderr, "%d %d\n", n, cpt);
    n += pas;
  } while (sortie_prog != NB_APP + NB_GRAPHE);
  
  return 0;
}
