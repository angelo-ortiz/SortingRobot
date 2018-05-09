#include "../include/src/ListeCircuit.h"

Cell_circuit *LCCreerCellule(LDC *L, int jmin, int jmax)
{
  Cell_circuit *cell = (Cell_circuit *) calloc(1, sizeof(Cell_circuit));
  if (cell == NULL) {
    fprintf(stderr, "Erreur lors de l'allocation d'une cellule\n");
    return NULL;
  }
  cell->L = L;
  cell->jmin = jmin;
  cell->jmax = jmax;
  return cell;
}

void LCInitialise(Lcircuit *lc)
{
  if (lc == NULL) {
    fprintf(stderr, "Erreur : la liste n'a pas ete allouee\n");
    return;
  }
  lc->nb_circuit = 0;
  lc->premier = NULL;
  lc->dernier = NULL;
}

int LCVide(Lcircuit *lc)
{
  if (lc == NULL) {
    fprintf(stderr, "Erreur : la liste n'a pas ete allouee\n");
    return -1;
  }
  return lc->premier == NULL /* && lc->dernier == NULL */;
}

void LCInsererEnTete(Lcircuit *lc, LDC *L, int jmin, int jmax)
{
  Cell_circuit *cell = NULL;
  int estVide = 0;
  // Si la liste n'a pas ete allouee ou il y a une erreur d'allocation
  if ((estVide = LCVide(lc)) < 0 || (cell = LCCreerCellule(L, jmin, jmax)) == NULL) {
    fprintf(stderr, "\tLa cellule n'a pas ete ajoutee\n");
    return;
  }
  // Si la liste est vide
  if (estVide == 1) {
    lc->dernier = cell;
  } // Si la liste contient deja au moins un element
  else {
    cell->suiv = lc->premier;
  }
  lc->premier = cell;
  lc->nb_circuit ++;
}

void LCInsererEnFin(Lcircuit *lc, LDC *L, int jmin, int jmax)
{
  Cell_circuit *cell = NULL;
  int estVide = 0;
  // Si la liste n'a pas ete allouee ou il y a une erreur d'allocation
  if ((estVide = LCVide(lc)) < 0 || (cell = LCCreerCellule(L, jmin, jmax)) == NULL) {
    fprintf(stderr, "\tLa cellule n'a pas ete ajoutee\n");
    return;
  }
  // Si la liste est vide
  if (estVide == 1) {
    lc->premier = cell;
  } // Si la liste contient deja au moins un element
  else {
    lc->dernier->suiv = cell;
  }
  lc->dernier = cell;
  lc->nb_circuit ++;
}

void LCEnleverCellule(Lcircuit *lc, Cell_circuit *cel)
{
  Cell_circuit *parcours = NULL;
  // Si la liste n'a pas ete allouee ou est vide
  if (LCVide(lc) != 0 || cel == NULL) {
    fprintf(stderr, "\tLa cellule n'a pas ete enlevee\n");
    return;
  }
  // Si c'est la premiere cellule
  if (cel == lc->premier) {
    lc->premier = cel->suiv;
  } //Quid d'une liste reduite a une seule cellule
  parcours = lc->premier;
  while (parcours->suiv != NULL) {
    if (parcours->suiv == cel) {
      // Si c'est la derniere cellule
      if (cel == lc->dernier) {
	lc->dernier = parcours;
      }
      parcours->suiv = cel->suiv;
      LDCdesalloue(cel->L);
      free(cel);
      lc->nb_circuit --; //dehors
      return;
    }
    parcours = parcours->suiv;
  }
}

void LCEnleverCelluleSuivante(Lcircuit *lc, Cell_circuit *prec)
{
  Cell_circuit *tmp = NULL;
  // Si la liste n'a pas ete allouee ou est vide
  if (LCVide(lc) != 0) {
    fprintf(stderr, "\tLa cellule n'a pas ete enlevee\n");
    return;
  }
  // Si c'est la premiere cellule
  if (prec == NULL) {
    tmp = lc->premier;
    lc->premier = tmp->suiv;
  } else {
    tmp = prec->suiv;
    prec->suiv = tmp->suiv;
  }
  // Si c'est la derniere cellule
  if (tmp == lc->dernier) {
    lc->dernier = prec;
  }
  LDCdesalloue(tmp->L);
  free(tmp->L);
  free(tmp);
  lc->nb_circuit --;
}

void LCAfficher(Lcircuit *lc)
{
  Cell_circuit *parcours = NULL;
  // Si la liste n'a pas ete allouee ou est vide
  if (LCVide(lc) != 0) {
    fprintf(stderr, "La liste est vide\n");
    return;
  }
  printf("Il y a %d circuits dans le graphe\n", lc->nb_circuit);
  parcours = lc->premier;
  while (parcours != NULL) {
    printf("(%d, %d) : ", parcours->jmin, parcours->jmax);
    LDCafficher(parcours->L);
    parcours = parcours->suiv;
  }
  printf("\n");
}

void LCDesalloue(Lcircuit *lc)
{
  Cell_circuit *parcours = NULL, *tmp = NULL;
  // Si la liste n'a pas ete allouee ou est vide
  if (LCVide(lc) != 0) {
    return;
  }
  parcours = lc->premier;
  while (parcours != NULL) {
    tmp = parcours;
    parcours = parcours->suiv;
    LDCdesalloue(tmp->L);
    free(tmp->L);
    free(tmp);
  }
  free(lc);
}
