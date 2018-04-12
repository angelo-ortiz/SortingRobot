#include "../include/src/ListeDC.h"

CelluleLDC *LDCCreerCellule(int i, int j)
{
  CelluleLDC *cell = (CelluleLDC *) calloc(1, sizeof(CelluleLDC));
  if (cell == NULL) {
    fprintf(stderr, "Erreur lors de l'allocation d'une cellule\n");
    return NULL;
  }
  cell->i = i;
  cell->j = j;
  return cell;
}

void LDCInitialise(LDC *ldc)
{
  if (ldc == NULL) {
    fprintf(stderr, "Erreur : la liste n'a pas ete allouee\n");
    return;
  }
  ldc->premier = NULL;
  ldc->dernier = NULL;
}

int LDCVide(LDC *ldc)
{
  if (ldc == NULL) {
    fprintf(stderr, "Erreur : la liste n'a pas ete allouee\n");
    return -1;
  }
  return ldc->premier == NULL /* && ldc->dernier == NULL */;
}

void LDCInsererEnTete(LDC *ldc, int i, int j)
{
  CelluleLDC *cell = NULL;
  int estVide = 0;
  // Si la liste n'a pas ete allouee ou il y a une erreur d'allocation
  if ((estVide = LDCVide(ldc)) < 0 || (cell = LDCCreerCellule(i, j)) == NULL) {
    fprintf(stderr, "\tLa cellule n'a pas ete ajoutee\n");
    return;
  }
  // Si la liste est vide
  if (estVide == 1) {
    ldc->dernier = cell;
  } // Si la liste contient deja au moins un element
  else {
    cell->suiv = ldc->premier;
    ldc->premier->prec = cell;
  }
  ldc->premier = cell;
}

void LDCInsererEnFin(LDC *ldc, int i, int j)
{
  CelluleLDC *cell = NULL;
  int estVide = 0;
  // Si la liste n'a pas ete allouee ou il y a une erreur d'allocation
  if ((estVide = LDCVide(ldc)) < 0 || (cell = LDCCreerCellule(i, j)) == NULL) {
    fprintf(stderr, "\tLa cellule n'a pas ete ajoutee\n");
    return;
  }
  // Si la liste est vide
  if (estVide == 1) {
    ldc->premier = cell;
  } // Si la liste contient deja au moins un element
  else {
    cell->prec = ldc->dernier;
    ldc->dernier->suiv = cell;
  }
  ldc->dernier = cell;
}

void LDCenleverCellule(LDC *ldc, CelluleLDC *cel)
{
  // Si la liste n'a pas ete allouee ou est vide
  if (LDCVide(ldc) != 0 || cel == NULL) {
    fprintf(stderr, "\tLa cellule n'a pas ete enlevee\n");
    return;
  }
  // Si c'est la premiere cellule
  if (cel == ldc->premier) {
    ldc->premier = cel->suiv;
  } else {
    cel->prec->suiv = cel->suiv;
  }
  // Si c'est la derniere cellule
  if (cel == ldc->dernier) {
    ldc->dernier = cel->prec;
  } else {
    cel->suiv->prec = cel->prec;
  }
  free(cel);
}

void LDCafficher(LDC *ldc)
{
  CelluleLDC *parcours = NULL;
  // Si la liste n'a pas ete allouee ou est vide
  if (LDCVide(ldc) != 0) {
    fprintf(stderr, "La liste est vide\n");
    return;
  }
  parcours = ldc->premier;
  while (parcours != NULL) {
    printf("(%d, %d)\t", parcours->i, parcours->j);
    parcours = parcours->suiv;
  }
  printf("\n");
}

void LDCdesalloue(LDC *ldc)
{
  CelluleLDC *parcours = NULL, *tmp = NULL;
  // Si la liste n'a pas ete allouee ou est vide
  if (LDCVide(ldc) != 0) {
    return;
  }
  parcours = ldc->premier;
  while (parcours != NULL) {
    tmp = parcours;
    parcours = parcours->suiv;
    free(tmp);
  }
  ldc->premier = NULL;
  ldc->dernier = NULL;
}

CelluleLDC *LDCrechercherPlusProcheCase(LDC *ldc, int i, int j)
{
  CelluleLDC *parcours = NULL, *min = NULL;
  int dist = 0, distMin = RAND_MAX;
  // Si la liste n'a pas ete allouee ou est vide
  if (LDCVide(ldc) != 0) {
    return NULL;
  }
  parcours = ldc->premier;
  while (parcours != NULL) {
    if ((dist = (abs(parcours->i - i) + abs(parcours->j - j))) < distMin) {
      min = parcours;
      distMin = dist;
    }
    parcours = parcours->suiv;
  }
  return min;
}
