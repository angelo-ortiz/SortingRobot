#ifndef __UNE_CASE_PAR_COULEUR__
#define __UNE_CASE_PAR_COULEUR__

#include "../lib/Graphe.h"
#include "ListeCircuit.h"
#include "Pile.h"
#include "AuPlusProche.h"

/**
 * Le nombre d'algorithmes de resolution par graphe du probleme
 * du robot trieur
 */
#define NB_GRAPHE 4

/**
 * Le coefficient definissant la susceptibilite de couper le
 * parcours d'un circuit : 
 *     coeff < 1. => rupture pour les circuit tres proches uniquement
 *     coeff > 1. => tendance a les couper facilement
 */
#define COEFF_COUPE 1.

/**
 * Fonction qui cherche un circuit commencant en (ir,jr) dans
 * le graphe <H> : elle renvoie 1 et affiche a l'ecran les coordonnees
 * de la case (i,j) si le chemin suivi nous amenant jusqu'a (i,j)
 * se referme en ce noeud ou s'il continue vers un successeur
 * de (i,j) et se referme eventuellement en (ir,jr), 0 sinon
 */
int Graphe_Rech_Circuit_rec(Graphe *H, int ir, int jr, int i, int j);

/**
 * Procedure qui cherche et affiche a l'ecran tous les circuits
 * dans le graphe <H> dans l'ordre inverse du parcours d'un circuit
 */
void Graphe_Rech_Circuit(Graphe *H);

/**
 * Fonction qui cherche un circuit commencant en (ir,jr) dans
 * le graphe <H> : elle renvoie 1 et ajoute le sommet associe a
 * la case (i,j) a la liste <ldc> si le chemin suivi nous amenant
 * jusqu'a (i,j) se referme en ce noeud ou s'il continue vers
 * un successeur de (i,j) et se referme eventuellement en (ir,jr),
 * 0 sinon
 */
int Graphe_Rech_Circuit_LC_rec(Graphe *H, LDC *ldc, int ir, int jr, int i, int j, int *jmin, int *jmax);

/**
 * Procedure qui cherche et ajoute dans <LC> tous les circuits
 * du graphe <H> dans l'ordre naturel de parcours d'un circuit
 */
void Graphe_Rech_Circuit_LC(Graphe *H, Lcircuit *LC);

/**
 * Fonction qui cherche un circuit commencant en (ir,jr) dans
 * le graphe <H> : elle renvoie 1 et ajoute le sommet associe a
 * la case (i,j) a la liste <ldc> si le chemin suivi nous amenant
 * jusqu'a (i,j) se referme en ce noeud ou s'il continue vers
 * le successeur de (i,j) le plus proche de celui-ci et se
 * referme eventuellement en (ir,jr), 0 sinon
 */
int Graphe_Rech_Circuit_Opt_rec(Graphe *H, LDC *ldc, int ir, int jr, int i, int j, int *jmin, int *jmax);

/**
 * Procedure qui cherche et ajoute dans <LC> tous les circuits
 * du graphe <H> tels que pour tout sommet, le prochain sommet
 * dans le circuit est le successeur le plus proche de celui-la
 */
void Graphe_Rech_Circuit_Opt(Graphe *H, Lcircuit *LC);

/**
 * Procedure verifiant les deux conditions suivantes sur <LC> :
 * - chaque circuit comporte les indices jmin et jmax correspondant
 *   a la case la plus a gauche et la plus a droite respectivement, et
 * - la liste est triee dans l'ordre croissant de jmin,
 * et affichant un message d'erreur a l'ecran en cas d'erreur
 * N.B. : Les fonctions de recherche de circuits tiennent compte de ces
 * deux conditions lors de leur execution
 */
void CalculJminJmax(Lcircuit *LC);

/**
 * Fonction qui insere le caractere <a> juste apres celui de la
 * cellule <c> dans <S> : si <c> est nul, il s'agit donc d'un ajout
 * en tete, et renvoie la cellule pointant vers le caractere ajoute
 */
Cell_char *Ajout_action_apres_c(Solution *S, Cell_char *c, int j, char a, Cell_char* *Tref);

/**
 * Fonction qui ajoute une suite de caracteres correspondant au
 * deplacement de la colonne <j> vers <l>, juste apres la cellule
 * <c> dans <S>
 */
Cell_char *PlusCourtChemin_apres_c(Solution *S, Cell_char *c, int j, int l, Cell_char* *Tref);

/**
 * TODO Cf fichier .c
 */
void Ajout_circuit_dans_solution(Solution *S, Cell_circuit *C, Cell_char *cell, Cell_char* *Tref, int *Jdroite);

/**
 * Procedure resolvant le probleme du robot trieur pour le
 * cas ou la grille est reduite a une ligne et qu'il y a
 * une case par couleur a travers l'algorithme de Daniel
 * Graf
 */
void algorithme_circuit_CasLigne1x1(Grille *G, Solution *S, int graine);

/**
 * Procedure resolvant le probleme du robot trieur a l'aide
 * d'un parcours sequentiel des circuits dans la grille
 * obtenus naivement par Graphe_Rech_Circuit_LC
 */
void algorithme_ucpc_naif(Grille *G, Solution *S, int graine);

/**
 * Procedure resolvant le probleme du robot trieur a l'aide
 * d'un parcours ameliore des circuits dans la grille
 * obtenus naivement par Graphe_Rech_Circuit_LC, i.e. a la
 * fin de chaque circuit on cherche le circuit dont le debut
 * est le sommet le plus proche du sommet correspondant a la
 * position courante du robot
 */
void algorithme_ucpc_ameliore(Grille *G, Solution *S, int graine);

/**
 * Procedure resolvant le probleme du robot trieur a l'aide
 * d'un parcours ameliore des circuits dans la grille
 * obtenus par Graphe_Rech_Circuit_Opt, i.e. a la fin de
 * chaque circuit on cherche le circuit dont le debut
 * est le sommet le plus proche du sommet correspondant a la
 * position courante du robot
 */
void algorithme_general(Grille *G, Solution *S, int graine);

/**
 * Procedure resolvant le probleme du robot trieur, reposant 
 * sur la methode generale ci-dessus et reprenant l'idee de
 * rupture des circuits de Daniel Graf : si un circuit est
 * "plus proche" que la case suivante dans le circuit courant,
 * on commence celui-la et on reprend eventuellement celui-ci
 */
void algorithme_general_coupes(Grille *G, Solution *S, int graine);

#endif
