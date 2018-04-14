#include <stdio.h>
#include <stdlib.h>

#include "../include/lib/Grille.h"
#include "../include/lib/Solution.h"
#include "../include/lib/Graphe.h"
#include "../include/lib/API_AffGrille.h"

int main(int argc, char* *argv){
  Grille G;
  Solution S;
  Graphe *H;
  int graine;

  if(argc!=5){
    printf("usage: %s <nb_lignes nb_colonnes nb_couleur graine>\n",argv[0]);
    return 1;
  }

  G.m=atoi(argv[1]);
  G.n=atoi(argv[2]);

  if (G.m>G.n){
    printf("Il doit y avoir plus de colonnes que de lignes.\n");
    exit(1);
  }
  G.nbcoul=atoi(argv[3]);
  if (G.nbcoul>G.m*G.n){
    printf("Il ne doit pas y avoir plus de couleurs que de cases.\n");
    exit(1);
  }

  graine=atoi(argv[4]);

  /* Generation de l'instance */
  Grille_allocation(&G);
  Gene_Grille(&G,graine);
  Solution_init(&S);
  
  H = (Graphe *) calloc(1, sizeof(Graphe));
  if (H == NULL) {
    Solution_desallocation(&S);
    Grille_desallocation(&G);
    fprintf(stderr, "Erreur lors de l'allocation d'un graphe\n");
    exit(1);
  }

  Graphe_creation(&G, H);
  Graphe_affiche(H);

  Solution_desallocation(&S);
  Grille_desallocation(&G);
  Graphe_desallocation(H);
  return 0;
}
