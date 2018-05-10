#ifndef __PILE__
#define __PILE__

#include <stdio.h>
#include <stdlib.h>

typedef struct element {
  void *cell; /* la donnee */
  struct element *prec; /* l'element precedent */
} Element;

typedef Element* Pile;

/**
 * Fonction qui renvoie
 *  1 si la liste est vide
 *  0 sinon
 */
int estPileVide(Pile p);

/**
 * Fonction qui alloue et cree un element
 * de la pile
 */
Pile creerElement(void *cell);

/**
 * Procedure qui cree d'abord un element
 * de la pile associe a <cell> et puis
 * l'ajoute en haut de la pile <*p>
 */
void empile(Pile *p, void *cell);

/**
 * Fonction qui retire l'element en haut
 * de la pile <*p> et le renvoie
 */
void *depile(Pile *p);

/**
 * Fonction qui renvoie l'element en haut
 * de la pile <p> sans l'enlever
 */
void *tetePile(Pile p);

#endif
