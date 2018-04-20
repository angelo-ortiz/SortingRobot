#ifndef __LISTE_CIRCUIT__
#define __LISTE_CIRCUIT__

#include <stdio.h>
#include <stdlib.h>
#include "ListeDC.h"

typedef struct cell_circuit {
  LDC *L; // la liste des cases composant le circuit
  int jmin, jmax; // TODO
  struct cell_circuit *suiv; // le circuit suivant
} Cell_circuit;

typedef struct {
  int nb_circuit; // le nombre de circuits
  Cell_circuit *premier; // le premier circuit
  Cell_circuit *dernier; // le dernier circuit
} Lcircuit;

/*
 * Fonction qui alloue et cree une cellule
 */
Cell_circuit *LCCreerCellule(LDC *L, int jmin, int jmax);

/*
 * Procedure qui initialise une liste, i.e. la liste
 * ne contient aucun element 
 */
void LCInitialise(Lcircuit *lc);

/*
 * Fonction qui renvoie
 * -1 si la liste est nulle
 * 1 si la liste existe mais est vide
 * 0 sinon
 */
int LCVide(Lcircuit *lc);

/*
 * Procedure qui cree d'abord une cellule avec les
 * coordonnees passees en argument et puis l'insere
 * en tete de liste
 */
void LCInsererEnTete(Lcircuit *lc, LDC *L, int jmin, int jmax);

/*
 * Procedure qui cree d'abord une cellule avec les
 * coordonnees passees en argument et puis l'insere
 * en fin de la liste
 */
void LCInsererEnFin(Lcircuit *lc, LDC *L, int jmin, int jmax);

/*
 * Procedure qui enleve une cellule a partir d'un pointeur
 * sur elle-meme
 */
void LCEnleverCellule(Lcircuit *lc, Cell_circuit *cel);

/*
 * Procedure qui enleve une cellule a partir d'un pointeur
 * sur la cellule precedente (NULL si c'est la premiere)
 */
void LCEnleverCelluleSuivante(Lcircuit *lc, Cell_circuit *prec);

/*
 * Procedure qui affiche la liste des coordonnees
 * sequentiellement
 */
void LCAfficher(Lcircuit *lc);

/*
 * Procedure qui desalloue toute la liste (si elle n'est 
 * pas vide) a la fin
 */
void LCDesalloue(Lcircuit *lc);

#endif
