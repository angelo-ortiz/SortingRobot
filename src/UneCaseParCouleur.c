#include "../include/src/UneCaseParCouleur.h"

sr_algorithme graphe_algorithmes[NB_GRAPHE] = {algorithme_ucpc_naif, \
					       algorithme_ucpc_ameliore, \
					       algorithme_general};

sr_algorithme gen_algorithmes[NB_APP+NB_GRAPHE] = {algorithme_naif, algorithme_circulaire, \
						   algorithme_parcouleur, algorithme_paravl, \
						   algorithme_ucpc_naif, \
						   algorithme_ucpc_ameliore, algorithme_general};

int Graphe_Rech_Circuit_rec(Graphe *H, int ir, int jr, int i, int j)
{
  int k, l;
  Arc *cour = NULL;
  
  /* 1 : recherche courante */
  H->Tsom[i][j]->visit = 1;
  cour = H->Tsom[i][j]->Lsucc;
  while (cour != NULL) {
    k = cour->succ->i;
    l = cour->succ->j;
    
    /* Si le sommet appartient au parcours courant 
     * et c'est le premier sommet du circuit
     * ou s'il est un noeud pas encore visite
     * mais par lequel passe le circuit cherche
     */	
    if ((H->Tsom[k][l]->visit == 1 && k == ir && l == jr) ||
	(H->Tsom[k][l]->visit == -1 && Graphe_Rech_Circuit_rec(H, ir, jr, k, l))) {
      printf("<-(%d,%d)", i, j);
      return 1;
    }
    cour = cour->suiv;
  }
  H->Tsom[i][j]->visit = -1;
  return 0;
}

void Graphe_Initialiser_Sommets(Graphe *H)
{
  int i, j;
  
  for (i = 0; i < H->m; ++i) {
    for (j = 0; j < H->n; ++j) {  
      if (H->Tsom[i][j] == NULL) {
	fprintf(stderr, "Erreur : le sommet correspondant a la case (%d,%d) n'a pas ete alloue\n", i, j);
	return;
      }
      
      /* Initialisation a -2 pour les sommets noirs */
      if (H->Tsom[i][j]->Lsucc == NULL) {
	H->Tsom[i][j]->visit = -2;
      }
    }
  }
}

void Graphe_Rech_Circuit(Graphe *H)
{
  int i, j;
  
  if (H == NULL) {
    fprintf(stderr, "Erreur : le graphe n'a pas ete alloue\n");
    return;
  }
  Graphe_Initialiser_Sommets(H);
  
  for (i = 0; i < H->m; ++i) {
    for (j = 0; j < H->n; ++j) {  
      if (H->Tsom[i][j]->visit != -1) {
	continue;
      }
      
      Graphe_Rech_Circuit_rec(H, i, j, i, j);
      printf("<|\n");
    }
  }
}

int Graphe_Rech_Circuit_LC_rec(Graphe *H, LDC *ldc, int ir, int jr, int i, int j, int *jmin, int *jmax)
{
  int k, l;
  Arc *cour = NULL;
  
  /* 1 : recherche courante */
  H->Tsom[i][j]->visit = 1;
  cour = H->Tsom[i][j]->Lsucc;
  while (cour != NULL) {
    k = cour->succ->i;
    l = cour->succ->j;
    
    /* Si le sommet appartient au parcours courant 
     * et c'est le premier sommet du circuit
     * ou s'il est un noeud pas encore visite
     * mais par lequel passe le circuit cherche
     */	
    if ((H->Tsom[k][l]->visit == 1 && k == ir && l == jr) ||
	(H->Tsom[k][l]->visit == -1 && Graphe_Rech_Circuit_LC_rec(H, ldc, ir, jr, k, l, jmin, jmax))) {
      LDCInsererEnTete(ldc, i, j);
      if (j < *jmin) {
	*jmin = j;
      }
      if (j > *jmax) {
	*jmax = j;
      }
      H->Tsom[i][j]->visit = 0;
      return 1;
    }
    cour = cour->suiv;
  }
  H->Tsom[i][j]->visit = -1;
  return 0;
}

void Graphe_Rech_Circuit_LC(Graphe *H, Lcircuit *LC)
{
  int i, j, jmin, jmax;
  LDC *ldc = NULL;
  
  if (H == NULL) {
    fprintf(stderr, "Erreur : le graphe n'a pas ete alloue\n");
    return;
  }
  Graphe_Initialiser_Sommets(H);
  
  if (LC == NULL) {
    fprintf(stderr, "Erreur : la liste de circuits n'a pas ete allouee\n");
    return;
  }
  
  for (i = 0; i < H->m; ++i) {
    for (j = 0; j < H->n; ++j) {  
      if (H->Tsom[i][j]->visit != -1) {
	continue;
      }
      
      ldc = (LDC *) calloc(1, sizeof(LDC));
      if (ldc == NULL) {
	fprintf(stderr, "Erreur lors de l'allocation d'une LDC\n");
	return;
      }
      
      LDCInitialise(ldc);
      jmin = 9999999;
      jmax = -1;
      Graphe_Rech_Circuit_LC_rec(H, ldc, i, j, i, j, &jmin, &jmax);
      LCInsererEnFin(LC, ldc, jmin, jmax);
    }
  }
}

int Graphe_Rech_Circuit_Opt_rec(Graphe *H, LDC *ldc, int ir, int jr, int i, int j, int *jmin, int *jmax)
{
  int k, l, kopt = -1, lopt = -1, dist, distMin = 9999999;
  int fin = 0, isMin = 0;
  Arc *cour = NULL;
  
  /* 1 : recherche courante */
  H->Tsom[i][j]->visit = 1;
  cour = H->Tsom[i][j]->Lsucc;
  while (cour != NULL) {
    k = cour->succ->i;
    l = cour->succ->j;
    
    /* Si le sommet appartient au parcours courant 
     * et c'est le premier sommet du circuit
     * ou s'il est un noeud pas encore visite
     * mais par lequel passe le circuit cherche
     */
    if (H->Tsom[k][l]->visit == 1 && k == ir && l == jr) {
      fin = 1;
      break;
    }
    if (H->Tsom[k][l]->visit == -1 && ((dist = abs(i-k) + abs(j-l)) < distMin)) {
      kopt = k;
      lopt = l;
      isMin = 1;
    }
    cour = cour->suiv;
  }
  if (fin || (isMin && Graphe_Rech_Circuit_LC_rec(H, ldc, ir, jr, kopt, lopt, jmin, jmax))) {
    LDCInsererEnTete(ldc, i, j);
    if (j < *jmin) {
      *jmin = j;
    }
    if (j > *jmax) {
	*jmax = j;
    }
    H->Tsom[i][j]->visit = 0;
    return 1;
  }
  H->Tsom[i][j]->visit = -1;
  return 0;
}

void Graphe_Rech_Circuit_Opt(Graphe *H, Lcircuit *LC)
{
  int i, j, jmin, jmax;
  LDC *ldc = NULL;
  
  if (H == NULL) {
    fprintf(stderr, "Erreur : le graphe n'a pas ete alloue\n");
    return;
  }
  Graphe_Initialiser_Sommets(H);
  
  if (LC == NULL) {
    fprintf(stderr, "Erreur : la liste de circuits n'a pas ete allouee\n");
    return;
  }
  
  for (i = 0; i < H->m; ++i) {
    for (j = 0; j < H->n; ++j) {  
      if (H->Tsom[i][j]->visit != -1) {
	continue;
      }
      
      ldc = (LDC *) calloc(1, sizeof(LDC));
      if (ldc == NULL) {
	fprintf(stderr, "Erreur lors de l'allocation d'une LDC\n");
	return;
      }
      
      LDCInitialise(ldc);
      jmin = 9999999;
      jmax = -1;
      Graphe_Rech_Circuit_Opt_rec(H, ldc, i, j, i, j, &jmin, &jmax);
      LCInsererEnFin(LC, ldc, jmin, jmax);
    }
  }
}

Lcircuit *LC_Initialiser(Grille *G, Graphe *H, void (*Rech_Circuit)(Graphe *, Lcircuit *))
{
  Lcircuit *LC;
  
  LC = (Lcircuit *) calloc(1, sizeof(Lcircuit));
  if (LC == NULL) {
    fprintf(stderr, "Erreur lors de l'allocation de la liste de circuits\n");
    return NULL;
  }
  
  LCInitialise(LC);
  Graphe_creation(G, H);
  Rech_Circuit(H, LC);s
  return LC;
}

void algorithme_ucpc_naif(Grille *G, Solution *S, int graine)
{
  Graphe *H = NULL;
  Lcircuit *LC = NULL;
  Cell_circuit *circuit = NULL;
  CelluleLDC *cell = NULL;
  int ir, jr;
  
  H = (Graphe *) calloc(1, sizeof(Graphe));
  if (H == NULL) {
    fprintf(stderr, "Erreur lors de l'allocation du graphe\n");
    return;
  }
  
  LC = LC_Initialiser(G, H, Graphe_Rech_Circuit_LC);
  if (LC == NULL) {
    free(H);
    return;
  }

  circuit = LC->premier;
  while (circuit != NULL) {
    cell = circuit->L->premier;
    ir = cell->i;
    jr = cell->j;
    while (cell != NULL) {
      echangerCouleur(G, S, cell->i, cell->j);
      cell = cell->suiv;
    }
    echangerCouleur(G, S, ir, jr);
    circuit = circuit->suiv;
  }
  
  LCDesalloue(LC);
  free(LC);
  Graphe_desallocation(H);
  
  if (S != NULL) {
    Ecriture_Disque(G->m, G->n, G->nbcoul, graine, S, "Graphe_Naif");
  }
}

void RecherchePlusProcheCircuit(Lcircuit *LC, int i, int j, Cell_circuit* *circuit)
{
  Cell_circuit *cour = NULL, *prec = NULL;
  int k, l, dist, distMin = 9999999;
  
  cour = LC->premier;
  while (cour != NULL) {
    k = cour->L->premier->i;
    l = cour->L->premier->j;
    if ((dist = abs(i-k) + abs(j-l)) < distMin) {
      *circuit = prec;
      distMin = dist;
    }
    prec = cour;
    cour = cour->suiv;
  }
}

void algorithme_circuit_plus_proche(Grille *G, Solution *S, int graine, void (*Rech_Circuit)(Graphe *, Lcircuit *), char *nom_algo)
{
  Graphe *H = NULL;
  Lcircuit *LC = NULL;
  Cell_circuit *circuit = NULL, *precedent = NULL;
  CelluleLDC *cell = NULL;
  int ir, jr;
  
  H = (Graphe *) calloc(1, sizeof(Graphe));
  if (H == NULL) {
    fprintf(stderr, "Erreur lors de l'allocation du graphe\n");
    return;
  }
  
  LC = LC_Initialiser(G, H, Rech_Circuit);
  if (LC == NULL) {
    free(H);
    return;
  }

  /* Traitement du premier circuit */
  circuit = LC->premier;
  cell = circuit->L->premier;
  ir = cell->i;
  jr = cell->j;
  while (cell != NULL) {
    echangerCouleur(G, S, cell->i, cell->j);
    cell = cell->suiv;
  }
  echangerCouleur(G, S, ir, jr);
  LCEnleverCelluleSuivante(LC, NULL);

  /*
   * Traitement du reste de circuits :
   * on recherche le circuit dont le debut
   * est le plus proche de celui du dernier 
   * circuit
   */
  while (!LCVide(LC)) {
    RecherchePlusProcheCircuit(LC, ir, jr, &precedent);
    if (precedent == NULL) {
      circuit = LC->premier;
    } else {
      circuit = precedent->suiv;
    }
    cell = circuit->L->premier;
    ir = cell->i;
    jr = cell->j;
    while (cell != NULL) {
      echangerCouleur(G, S, cell->i, cell->j);
      cell = cell->suiv;
    }
    echangerCouleur(G, S, ir, jr);
    LCEnleverCelluleSuivante(LC, precedent);
  }

  //LCDesalloue(LC);
  free(LC);
  Graphe_desallocation(H);

  if (S != NULL) {
    Ecriture_Disque(G->m, G->n, G->nbcoul, graine, S, nom_algo);
  }
}

void algorithme_ucpc_ameliore(Grille *G, Solution *S, int graine)
{
  algorithme_circuit_plus_proche(G, S, graine, Graphe_Rech_Circuit_LC, "Graphe_ameliore");
}

void algorithme_general(Grille *G, Solution *S, int graine)
{
  algorithme_circuit_plus_proche(G, S, graine, Graphe_Rech_Circuit_Opt, "Graphe_general");
}
