#ifndef __GRAPHE__
#define __GRAPHE__

#include <stdio.h>
#include <stdlib.h>
#include "../src/ListeDC.h"
#include "Grille.h"

typedef struct sommet Sommet;

typedef struct arete {
  struct sommet *succ;
  struct arete *suiv;
} Arc;

struct sommet{
  int i,j;
  Arc *Lsucc;
  int visit;
};


typedef struct{
  int n,m;
  Sommet** *Tsom;
} Graphe;


void Graphe_init(Graphe *H, int m, int n);

void Graphe_ajout_arc(Graphe *H, int i1, int j1, int i2, int j2);

void Graphe_creation(Grille *G, Graphe *H);

void Graphe_affiche(Graphe *H);

void Graphe_desallocation(Graphe *H);

#endif
