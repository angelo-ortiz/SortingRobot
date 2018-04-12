#ifndef __UNE_CASE_PAR_COULEUR__
#define __UNE_CASE_PAR_COULEUR__

#include "../lib/Graphe.h"
#include "ListeCircuit.h"
#include "AuPlusProche.h"

/*
 * Le nombre de versions de l'algorithme de resolution par graphe
 */
#define NB_GRAPHE 3

/*
 * Pointeur sur fonction, ici un algorithme de resolution
 */
typedef void (*gen_algorithme) (Grille* G, Solution* S, int graine);

int Graphe_Rech_Circuit_rec(Graphe *H, int ir, int jr, int i, int j);

void Graphe_Rech_Circuit(Graphe *H);

int Graphe_Rech_Circuit_LC_rec(Graphe *H, LDC *ldc, int ir, int jr, int i, int j, int *jmin, int *jmax);

void Graphe_Rech_Circuit_LC(Graphe *H, Lcircuit *LC);

int Graphe_Rech_Circuit_Opt_rec(Graphe *H, LDC *ldc, int ir, int jr, int i, int j, int *jmin, int *jmax);

void Graphe_Rech_Circuit_Opt(Graphe *H, Lcircuit *LC);

void algorithme_ucpc_naif(Grille *G, Solution *S, int graine);

void algorithme_ucpc_ameliore(Grille *G, Solution *S, int graine);

void algorithme_general(Grille *G, Solution *S, int graine);

#endif
