#ifndef __UNE_CASE_PAR_COULEUR__
#define __UNE_CASE_PAR_COULEUR__

#include "../lib/Graphe.h"
#include "ListeCircuit.h"
#include "AuPlusProche.h"

/*
 * Le nombre d'algorithmes de resolution par graphe du probleme
 * du robot trieur
 */
#define NB_GRAPHE 3

/*
 * Fonction qui cherche un circuit commencant en (ir,jr) dans
 * le graphe H : elle renvoie 1 et affiche a l'ecran les coordonnees
 * de la case (i,j) si le chemin suivi nous amenant jusqu'a (i,j)
 * se referme en ce noeud ou s'il continue vers un successeur
 * de (i,j) et se referme eventuellement en (ir,jr), 0 sinon
 */
int Graphe_Rech_Circuit_rec(Graphe *H, int ir, int jr, int i, int j);

/*
 * Procedure qui cherche et affiche a l'ecran tous les circuits
 * dans le graphe H dans l'ordre inverse du parcours d'un circuit
 */
void Graphe_Rech_Circuit(Graphe *H);

/*
 * Fonction qui cherche un circuit commencant en (ir,jr) dans
 * le graphe H : elle renvoie 1 et ajoute le sommet associe a
 * la case (i,j) a la liste <ldc> si le chemin suivi nous amenant
 * jusqu'a (i,j) se referme en ce noeud ou s'il continue vers
 * un successeur de (i,j) et se referme eventuellement en (ir,jr),
 * 0 sinon
 */
int Graphe_Rech_Circuit_LC_rec(Graphe *H, LDC *ldc, int ir, int jr, int i, int j, int *jmin, int *jmax);

/*
 * Procedure qui cherche et ajoute dans LC tous les circuits
 * du graphe H dans l'ordre naturel de parcours d'un circuit
 */
void Graphe_Rech_Circuit_LC(Graphe *H, Lcircuit *LC);

/*
 * Fonction qui cherche un circuit commencant en (ir,jr) dans
 * le graphe H : elle renvoie 1 et ajoute le sommet associe a
 * la case (i,j) a la liste <ldc> si le chemin suivi nous amenant
 * jusqu'a (i,j) se referme en ce noeud ou s'il continue vers
 * le successeur de (i,j) le plus proche de celui-ci et se
 * referme eventuellement en (ir,jr), 0 sinon
 */
int Graphe_Rech_Circuit_Opt_rec(Graphe *H, LDC *ldc, int ir, int jr, int i, int j, int *jmin, int *jmax);

/*
 * Procedure qui cherche et ajoute dans LC tous les circuits
 * du graphe H tels que pour tout sommet, le prochain sommet
 * dans le circuit est le successeur le plus proche de celui-la
 */
void Graphe_Rech_Circuit_Opt(Graphe *H, Lcircuit *LC);

void CalculJminJmax(Lcircuit *LC);

Cell_char *Ajout_action_apres_c(Solution *S, Cell_char *c, int j, char a, Cell_char* *Tref);

Cell_char *PlusCourtChemin_apres_c(Solution *S, Cell_char *c, int j, int l, Cell_char* *Tref);

void Ajout_circuit_dans_solution(Solution *S, Cell_circuit *C, Cell_char* *Tref, int *Jdroite);

void algorithme_circuit_CasLigne1x1(Grille *G, Solution *S, int graine);

/*
 * Procedure resolvant le probleme du robot trieur a l'aide
 * d'un parcours sequentiel des circuits dans la grille
 * obtenus naivement par Graphe_Rech_Circuit_LC
 */
void algorithme_ucpc_naif(Grille *G, Solution *S, int graine);

/*
 * Procedure resolvant le probleme du robot trieur a l'aide
 * d'un parcours ameliore des circuits dans la grille
 * obtenus naivement par Graphe_Rech_Circuit_LC, i.e. a la
 * fin de chaque circuit on cherche le circuit dont le debut
 * est le sommet le plus proche du sommet correspondant a la
 * position courante du robot
 */
void algorithme_ucpc_ameliore(Grille *G, Solution *S, int graine);

/*
 * Procedure resolvant le probleme du robot trieur a l'aide
 * d'un parcours ameliore des circuits dans la grille
 * obtenus par Graphe_Rech_Circuit_Opt, i.e. a la fin de
 * chaque circuit on cherche le circuit dont le debut
 * est le sommet le plus proche du sommet correspondant a la
 * position courante du robot
 */
void algorithme_general(Grille *G, Solution *S, int graine);

#endif
