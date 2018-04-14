#include <stdio.h>
#include <stdlib.h>

#include "../include/lib/Grille.h"
#include "../include/lib/Solution.h"
#include "../include/lib/Graphe.h"
#include "../include/lib/API_AffGrille.h"
#include "../include/src/UneCaseParCouleur.h"

void menu()
{
  printf("Voulez-vous utiliser des listes chainees pour afficher les circuits dans graphe ?\n");
  printf("0.- Non\n");
  printf("1.- Oui\n");
  printf("Entrez votre choix : ");
}

int main(int argc, char* *argv){
  Grille G;
  Solution S;
  Graphe *H;
  Lcircuit *LC;
  int graine;
  unsigned int ch;
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
    fprintf(stderr, "Erreur lors de l'allocation du graphe\n");
    exit(1);
  }

  LC = (Lcircuit *) calloc(1, sizeof(Lcircuit));
  if (LC == NULL) {
    Solution_desallocation(&S);
    Grille_desallocation(&G);
    free(H);
    fprintf(stderr, "Erreur lors de l'allocation de la liste de circuits\n");
    exit(1);
  }

  do {
    menu();
    scanf(" %u", &ch);
  } while (ch > 1);

  Graphe_creation(&G, H);
  
  /*
   * Recherche et affichage de circuits a l'aide de listes chainees
   */
  if (ch) {
    printf("\n\nAvec listes chainees : \n");
    LCInitialise(LC);
    Graphe_Rech_Circuit_LC(H, LC);
    LCAfficher(LC);
    LCDesalloue(LC);
  }
  /*
   * Affichage de circuits
   */
  else {
    printf("Sans listes chainees : \n");
    Graphe_Rech_Circuit(H);
  }
  
  Solution_desallocation(&S);
  Grille_desallocation(&G);
  Graphe_desallocation(H);
  free(LC);
  return 0;
}
