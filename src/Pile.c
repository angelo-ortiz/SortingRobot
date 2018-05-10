#include "Pile.h"

int estPileVide(Pile p)
{
  return p == NULL;
}

Pile creerElement(void *cell)
{
  Pile elem = (Element *) calloc(1, sizeof(Element));
  if (elem == NULL) {
    fprintf(stderr, "Erreur lors de l'allocation d'un element de la pile\n");
    return NULL;
  }
  elem->cell = cell;
  return elem;
}

void empile(Pile *p, void *cell)
{
  Pile elem = creerElement(cell);
  if (elem == NULL) {
    return;
  }
  elem->prec = *p;
  *p = elem;
}

void *depile(Pile *p)
{
  void *cell = NULL;
  Pile tmp = NULL;
  if (estPileVide(*p)) {
    fprintf(stderr, "Erreur : la pile est vide\n");
    return NULL;
  }
  cell = (*p)->cell;
  tmp = *p;
  *p = tmp->prec;
  free(tmp);
  return cell;
}

void *tetePile(Pile p)
{
  if (estPileVide(p)) {
    fprintf(stderr, "Erreur : la pile est vide\n");
    return NULL;
  }
  return p->cell;
}
