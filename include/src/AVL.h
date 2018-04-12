#ifndef __AVL__
#define __AVL__

#include <stdio.h>
#include <stdlib.h>

typedef struct s_avl {
  int content; /* contenu du noeud */
  int hauteur; /* hauteur de l'arbre */
  struct s_avl *fg; /* fils gauche */
  struct s_avl *fd; /* fils droit */
} AVL;

/*
 * Procedure qui renvoie la hauteur des deux fils d'un noeud
 */
void hauteurFils(AVL *avl, int *hg, int *hd);

/*
 * Procedure qui met a jour la hauteur d'un noeud
 */
void majHauteur(AVL *avl);

/*
 * Fonction qui alloue et cree un nouveau noeud avec la donnee 
 * content et les sous-arbres fg et fd
 */
AVL *creerNoeud(int content, AVL *fg, AVL *fd);

/* 
 * Fonction qui renvoie l'arbre obtenu apres une rotation droite
 */
AVL *rotDroite(AVL *root);

/*
 * Fonction qui renvoie l'arbre obtenu apres une rotation gauche
 */
AVL *rotGauche(AVL *root);

/*
 * Fonction qui renvoie l'arbre obtenu apres une double rotation droite
 */
AVL *doubleRotDroite(AVL *root);

/*
 * Fonction qui reequilibre l'arbre AVL
 */
AVL *equilibrer(AVL *avl);

/*
 * Fonction qui insere une valeur dans l'arbre AVL
 */
AVL *insererNoeud(AVL *avl, int val);

/*
 * Fonction qui supprime le noeud dont la valeur est la plus grande
 * dans l'arbre et qui renvoie cette valeur
 */
AVL *supprimerMax(AVL *avl, int *valMax);

/*
 * Fonction qui enleve un noeud de l'arbre AVL
 */
AVL *enleverNoeud(AVL *avl, int val);

/*
 * Procedure d'affichage de la valeur et la hauteur de chaque noeud de 
 * l'arbre AVL selon un parcours infixe
 */
void AVLafficher(AVL *avl);

/*
 * Procedure qui desalloue tout l'arbre (s'il n'est pas vide) a la fin
 */
void AVLdesalloue(AVL *avl);

/*
 * Fonction qui rend la valeur la plus proche de l dans l'arbre AVL,
 * i.e. la colonne de la case la plus proche de la case (i,l) et ayant 
 * une piece de la meme couleur sachant que l'arbre AVL correspond aux
 * cases de la i-ieme ligne ayant une couleur precise  
 */
int AVLrechercherPlusProcheCase(AVL *avl, int l);

#endif
