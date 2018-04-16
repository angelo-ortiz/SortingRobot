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
 * Fonction qui cherche un circuit commencant en (ir, jr) dans
 * le graphe H et qui renvoie 1 et affiche a l'ecran les coordonnees
 * d'un sommet si la cherche et reussie, 0 sinon
 */
int Graphe_Rech_Circuit_rec(Graphe *H, int ir, int jr, int i, int j);

/*
 * Fonction qui cherche et affiche a l'ecran tous les circuits
 * dans le graphe H
 */
void Graphe_Rech_Circuit(Graphe *H);

/*
 * Fonction qui cherche un circuit commencant en (ir, jr) dans
 * le graphe H et qui renvoie 1 et ajoute le sommet a la liste
 * correspondant au circuit si ledit sommet si la cherche et reussie, 0 sinon
 */
int Graphe_Rech_Circuit_LC_rec(Graphe *H, LDC *ldc, int ir, int jr, int i, int j, int *jmin, int *jmax);

/*
 * Fonction qui cherche et ajoute dans LC tous les circuits
 * du graphe H
 */
void Graphe_Rech_Circuit_LC(Graphe *H, Lcircuit *LC);

int Graphe_Rech_Circuit_Opt_rec(Graphe *H, LDC *ldc, int ir, int jr, int i, int j, int *jmin, int *jmax);

/*
 * Fonction qui cherche et ajoute dans LC tous les circuits
 * du graphe H ayant la distance entre deux sommets adjacents
 * la plus petite possible
 */
void Graphe_Rech_Circuit_Opt(Graphe *H, Lcircuit *LC);

void algorithme_ucpc_naif(Grille *G, Solution *S, int graine);

void algorithme_ucpc_ameliore(Grille *G, Solution *S, int graine);

void algorithme_general(Grille *G, Solution *S, int graine);

#endif
