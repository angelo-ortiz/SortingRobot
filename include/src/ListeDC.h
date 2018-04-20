#ifndef __LISTE_DC__
#define __LISTE_DC__

#include <stdio.h>
#include <stdlib.h>

typedef struct celluleLDC {
  int i, j; // les coordonnees d'une case
  struct celluleLDC *prec; // l'element precedent
  struct celluleLDC *suiv; // l'element suivant
} CelluleLDC;

typedef struct {
  CelluleLDC *premier; // le premier element de la liste
  CelluleLDC *dernier; // le dernier element de la liste
} LDC;

/*
 * Fonction qui alloue et cree une cellule
 */
CelluleLDC *LDCCreerCellule(int i, int j);

/*
 * Procedure qui initialise une liste, i.e. la liste
 * ne contient aucun element 
 */
void LDCInitialise(LDC *ldc);

/*
 * Fonction qui renvoie
 * -1 si la liste est nulle
 * 1 si la liste existe mais est vide
 * 0 sinon
 */
int LDCVide(LDC *ldc);

/*
 * Procedure qui cree d'abord une cellule avec les
 * coordonnees passees en argument et puis l'insere
 * en tete de liste
 */
void LDCInsererEnTete(LDC *ldc, int i, int j);

/*
 * Procedure qui cree d'abord une cellule avec les
 * coordonnees passees en argument et puis l'insere
 * en fin de liste
 */
void LDCInsererEnFin(LDC *ldc, int i, int j);

/*
 * Procedure qui enleve une cellule a partir d'un pointeur
 * sur elle-meme
 */
void LDCenleverCellule(LDC *ldc, CelluleLDC *cel);

/*
 * Procedure qui affiche la liste des coordonnees
 *  sequentiellement
 */
void LDCafficher(LDC *ldc);

/*
 * Procedure qui desalloue tous les elements de la liste
 * (si elle n'est pas vide) a la fin
 */
void LDCdesalloue(LDC *ldc);

/*
 * Fonction qui renvoie la cellule correspondant a la premiere
 * case trouvee qui est la plus proche de la case (i, j)
 */
CelluleLDC *LDCrechercherPlusProcheCase(LDC *ldc, int i, int j);

#endif
