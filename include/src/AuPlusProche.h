#ifndef __AU_PLUS_PROCHE__
#define __AU_PLUS_PROCHE__

#include <stdio.h>
#include <stdlib.h>
#include "../lib/Solution.h"
#include "../lib/Grille.h"
#include "AVL.h"
#include "ListeDC.h"

/*
 * Le nombre de versions de l'algorithme au plu proche
 */
#define NB_APP 4

/*
 * Pointeur sur fonction, ici une version de l'algorithme 
 * au plus proche
 */
typedef void (*app_algorithme) (Grille* G, Solution* S, int graine);

/*
 * Fonction qui renvoie
 * -1 si le robot ne porte pas de piece
 * la couleur entre [|0, nb_couleurs-1|] s'il en a une
 */
int couleurPieceRobot(Grille *G);

/*
 * Fonction qui renvoie vrai ssi le robot porte
 * effectivement une piece
 */
int robotPortePiece(Grille *G);

/*
 * Fonction qui renvoie vrai ssi la case (i,j) est noire,
 * i.e. les couleurs du fond et de la piece coincident
 */
int estCaseNoire(Grille *G, int i, int j);

/*
 * Fonction qui renvoie vrai ssi la piece de la case (i,j)
 * est noire
 */
int estPieceNoire(Grille *G, int i, int j);

/*
 * Fonction qui renvoie vrai ssi la case (i,j) est non noire
 * et sa couleur est c
 */
int estCaseCouleur_c(Grille *G, int i, int j, int c);

/*
 * Fonction qui renvoie vrai ssi la case (i,j) est non noire
 * et sa piece est aussi non noire
 */
int estCaseNonNoire(Grille *G, int v, int h);

/*
 * Procedure qui trouve le plus court chemin de la case (i,j)
 * vers la case (k,l) avec d'abord un deplacement horizontal et
 * puis un deplacement vertical, et qui ecrit ces deplacements
 * dans la solution passee en argument si est non nulle
 */
void PlusCourtChemin(Solution *S, int i, int j, int k, int l);

/*
 * Procedure qui recherche naivement la case non noire (k,l) 
 * la plus proche de la case (i,j) au sens de l'algorithme
 * au plus proche ayant la meme couleur, i.e. on parcourt
 * toute la grille avant de renvoyer la bonne case 
 */
void RechercheCaseNaif_c(Grille *G, int c, int i, int j, int *k, int *l);

/*
 * Procedure qui recherche naivement la case non noire (k,l)
 * la plus proche de la case (i,j) possedant une piece non noire,
 * i.e. on parcourt toute la grille avant de renvoyer la bonne case 
 */
void RechercheCaseNaif_nn(Grille *G, int i, int j, int *k, int *l);

/*
 * Procedure permettant de deplacer le robot vers la case (k,l)
 * dans un premier instant, puis d'echanger les pieces du robot
 * et de ladite case
 */
void echangerCouleur(Grille *G, Solution *S, int k, int l);

/*
 * Procedure implementant la version naive de l'algorithme au
 * plus proche, i.e. on fait un parcours de toute la grille
 * pour trouver la case ou le robot doit deposer la piece
 * qu'il porte
 */
void algorithme_naif(Grille *G, Solution *S, int graine);

/*
 * Procedure qui recherche circulairement la case non noire (k,l) 
 * la plus proche de la case (i,j) au sens de l'algorithme
 * au plus proche ayant la meme couleur, i.e. on parcourt toutes 
 * les cases se trouvant a une meme distance (r) de (i,j) et si 
 * aucune case ne remplit les conditions, on augmente le rayon 
 * (r) pour continuer la recherche
 */
void RechercheCaseCirculaire_c(Grille *G, int c, int i, int j, int *k, int *l);

/*
 * Procedure qui recherche circulairement la case non noire (k,l)
 * la plus proche de la case (i,j) possedant une piece non noire,
 * i.e. on parcourt toutes les cases se trouvant a une meme distance (r)
 * de (i,j) et si aucune case ne remplit les conditions, on augmente
 * le rayon (r) pour continuer la recherche
 */
void RechercheCaseCirculaire_nn(Grille *G, int i, int j, int *k, int *l);

/*
 * Procedure implementant la version circulaire de l'algorithme
 * au plus proche, i.e. on recherche circulairement autour du
 * robot la case ou le robot doit deposer la piece qu'il porte
 */
void algorithme_circulaire(Grille *G, Solution *S, int graine);

/*
 * Procedure implementant la version par couleur de l'algorithme au
 * plus proche, i.e. on range tout d'abord les cases dans des
 * listes doublement chainees selon leur couleur, puis pour chaque
 * recherche de destination pour le robot on n'a qu'a chercher la
 * case la plus proche dans la liste correspondant a la couleur de
 * la piece que porte le robot
 */
void algorithme_parcouleur(Grille *G, Solution *S, int graine);

/*
 * Procedure qui recherche a l'aide d'arbres AVL la case non noire
 * (k,l) la plus proche de la case (i,j) au sens de l'algorithme au
 * plus proche ayant la meme couleur, i.e. on recherche la case la
 * plus proche dans chaque ligne et choisit la plus proche d'entre
 * elles
 */
void RechercheCaseAVL_c(Grille *G, int c, AVL** *M, int i, int j, int *k, int *l);

/*
 * Procedure implementant la version par AVL de l'algorithme au
 * plus proche, i.e. on range tout d'abord les cases dans des arbres
 * AVL selon leur ligne et couleur, puis pour chaque recherche de
 * destination pour le robot on cherche la case la plus proche de
 * chaque ligne ayant la couleur de la piece portee par le robot et
 * on choisit celle qui est la plus proche entre elles
 */
void algorithme_paravl(Grille *G, Solution *S, int graine);

#endif
