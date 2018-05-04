#ifndef __PILE__
#define __PILE__

#include <stdio.h>
#include <stdlib.h>
/* #include "ListeDC.h" */

typedef struct element {
  void *cell;
  struct element *prec;
} Element;

typedef Element* Pile;

int estPileVide(Pile p);

Pile creerElement(void *cell);

void empile(Pile *p, void *cell);

void *depile(Pile *p);

void *tetePile(Pile p);

/* void affichePile(Pile p); */

#endif
