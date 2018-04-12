#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/src/AVL.h"

int doit_inserer(double proba)
{
  return ((double) rand()) / ((double) RAND_MAX) < proba;
}

int main(int argc, char* *argv)
{
  int i, el, taille, nb_max;
  AVL* avl = NULL;

  if (argc != 3) {
    fprintf(stderr, "Usage: %s <entier_maximum> <nombre_elements>", argv[0]);
    exit(1);
  }
  nb_max = atoi(argv[1]);
  taille = atoi(argv[2]);
  srand(time(NULL));
  
  for (i = 0; i < taille; ++i) {
    el = rand() % nb_max;
    if (doit_inserer(0.5)) {
      avl = insererNoeud(avl, el);
      printf("Essai d'ajout de (el = %d)\n", el);
    } else {
      avl = enleverNoeud(avl, el);
      printf("Essai de suppression de (el = %d)\n", el);
    }
  }
  
  AVLafficher(avl);
  AVLdesalloue(avl);
  return 0;
}
