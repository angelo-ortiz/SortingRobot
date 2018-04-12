#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/lib/Grille.h"
#include "../include/lib/Solution.h"
#include "../include/src/AuPlusProche.h"
#include "../include/src/UneCaseParCouleur.h"
#include "../include/lib/API_AffGrille.h"

extern gen_algorithme gen_algorithmes[NB_APP + NB_GRAPHE];

void menu()
{
  printf("==================== Type de solveur ====================\n");
  printf("0.- Sortir\n");
  printf("1.- Naif\n");
  printf("2.- Circulaire\n");
  printf("3.- Par couleur\n");
  printf("4.- Par AVL\n");
  printf("5.- Par graphe (une case par couleur / naif)\n");
  printf("6.- Par graphe (une case par couleur / ameliore)\n");
  printf("7.- Par graphe (general)\n");
  printf("Entrez votre choix [0-%d] : ", NB_APP + NB_GRAPHE);
}

int main(int argc,char**argv){
  Grille G;
  Solution S;
  int graine;
  int ch;
  clock_t temps_initial, temps_final;
  double temps_exe;
  char *nom_algorithmes[NB_APP + NB_GRAPHE] = {"naif", "circulaire", "par couleur", \
					       "par avl", "par graphe naif", \
					       "par graphe ameliore", "par graphe general"};
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
        
  /* Boucle generale de resolution du jeu */
  do {
    menu();
    scanf(" %d", &ch);
    if (ch < 0 || ch > NB_APP + NB_GRAPHE) {
      fprintf(stderr, "Erreur : Veuillez saisir un choix valide [0-%d]\n\n", NB_APP + NB_GRAPHE);
    } else if (ch != 0) {
      Gene_Grille(&G,graine);
      Solution_init(&S);

      temps_initial = clock();
      (gen_algorithmes[ch-1])(&G, &S, graine);
      temps_final = clock();
      temps_exe = ((double) (temps_final - temps_initial))/((double) CLOCKS_PER_SEC);
      printf("Le solveur %s a fini en %7.6e secondes\n\n", nom_algorithmes[ch-1], temps_exe);

      Solution_desallocation(&S);
    }
  } while (ch != 0);
  
  Grille_desallocation(&G);
  return 0;
}
