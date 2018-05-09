#include "../include/src/UneCaseParCouleur.h"

sr_algorithme graphe_algorithmes[NB_GRAPHE] = {algorithme_ucpc_naif, \
					       algorithme_ucpc_ameliore, \
					       algorithme_general, \
					       algorithme_general_coupes};

sr_algorithme gen_algorithmes[NB_APP+NB_GRAPHE+1] = {algorithme_naif, algorithme_circulaire, \
						     algorithme_parcouleur, algorithme_paravl, \
						     algorithme_circuit_CasLigne1x1, \
						     algorithme_ucpc_naif, \
						     algorithme_ucpc_ameliore, algorithme_general, \
						     algorithme_general_coupes};

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
    
    /* Si (ir,jr) est un successeur de (i,j)
     * ou si un successeur de (i,j) n'a pas
     * encore ete visite mais que le chemin
     * suivi jusqu'ici et continuant vers
     * ledit successeur se referme eventuellement
     * en (ir,jr), on affiche les coordonnees
     * (i,j) a l'ecran
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

/*
 * Procedure initialisant a -2 tous les sommets
 * correspondant a une case noire dans la grille
 * du jeu
 */
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
    
    /* Si (ir,jr) est un successeur de (i,j)
     * ou si un successeur de (i,j) n'a pas
     * encore ete visite mais que le chemin
     * suivi jusqu'ici et continuant vers
     * ledit successeur se referme eventuellement
     * en (ir,jr), on ajoute le sommet (i,j)
     * a la liste correspondant au circuit
     * commencant en (ir,jr)
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
  int fin = 0, finSav = 0, isMin = 0;
  Arc *cour = NULL;
  
  /* 1 : recherche courante */
  H->Tsom[i][j]->visit = 1;
  cour = H->Tsom[i][j]->Lsucc;
  while (cour != NULL) {
    k = cour->succ->i;
    l = cour->succ->j;

    /* Si (ir,jr) est un successeur de (i,j) */
    if (H->Tsom[k][l]->visit == 1 && k == ir && l == jr) {
      fin = 1;
    }
    
    /* 
     * On met a jour le sommet le plus proche de (i,j) n'ayant pas encore
     * ete visite ou refermant le circuit
     */
    if ((fin || H->Tsom[k][l]->visit == -1) && ((dist = abs(i-k) + abs(j-l)) < distMin)) {
      kopt = k;
      lopt = l;
      isMin = 1;
      distMin = dist;
      finSav = 0;
      if (fin == 1) {
	finSav = 1;
	fin = 0;
      }
    }
    cour = cour->suiv;
  }
  /* Si (ir,jr) est un successeur de (i,j)
   * ou s'il y a un successeur de (i,j)
   * etant le plus proche de (i,j) et n'ayant
   * pas encore ete visite mais que le chemin
   * suivi jusqu'ici et continuant vers ledit
   * successeur se referme eventuellement
   * en (ir,jr), on ajoute le sommet (i,j)
   * a la liste correspondant au circuit
   * commencant en (ir,jr)
   */
  if (finSav || (isMin && Graphe_Rech_Circuit_Opt_rec(H, ldc, ir, jr, kopt, lopt, jmin, jmax))) {
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

void CalculJminJmax(Lcircuit *LC)
{
  Cell_circuit *cell = NULL;
  if (LC == NULL) {
    fprintf(stderr, "La liste de circuits est vide\n");
    return;
  }
  /*
   * Si la liste est vide ou contient un element,
   * elle est deja triee
   */
  if (LC->premier == LC->dernier) {
    return;
  }
  cell = LC->premier;
  while (cell->suiv != NULL) {
    if (cell->jmin > cell->suiv->jmin) {
      fprintf(stderr, "Erreur : liste non triee\n");
      return;
    }
    cell = cell->suiv;
  }
}

Cell_char *Ajout_action_apres_c(Solution *S, Cell_char *c, int j, char a, Cell_char* *Tref)
{
  Cell_char *cell = NULL;
  if (S == NULL) {
    fprintf(stderr, "La solution n'a pas ete allouee\n");
    return NULL;
  }
  cell = (Cell_char *) calloc(1, sizeof(Cell_char));
  if (cell == NULL) {
    fprintf(stderr, "Erreur lors de l'allocation d'une cellule\n");
    return NULL;
  }
  cell->a = a;
  if (c == NULL) {
    // Si la liste est vide
    if (S->prem == S->dern) {
      S->dern = cell;
    } // Si la liste contient deja au moins un element
    else {
      cell->suiv = S->prem;
    }
    S->prem = cell;
  } else {
    cell->suiv = c->suiv;
    c->suiv = cell;
  }
  S->cptr_pas ++;
  Tref[j] = cell;
  return cell;
}

Cell_char *PlusCourtChemin_apres_c(Solution *S, Cell_char *c, int j, int l, Cell_char* *Tref)
{
  int i, pas;
  char a;
  Cell_char *cell = c;
  if (l > j) {
    pas = 1;
    a = 'R';
  } else {
    pas = -1;
    a = 'L';
  }
  for (i = j; i != l; i += pas) {
    cell = Ajout_action_apres_c(S, cell, i+pas, a, Tref);
  }
  return cell;
}

void Ajout_circuit_dans_solution(Solution *S, Cell_circuit *C, Cell_char* *Tref, int *Jdroite)
{
  int j, l;
  Cell_char *circ = Tref[C->jmin];
  CelluleLDC *cell = NULL;
  if (S == NULL) {
    fprintf(stderr, "La solution n'a pas ete allouee\n");
    return;
  }
  if (C == NULL) {
    fprintf(stderr, "Le circuit est nul\n");
    return;
  }
  
  cell = C->L->premier;
  if (circ == NULL || circ->a != 'S') {
    circ = Ajout_action_apres_c(S, circ, cell->j, 'S', Tref);
  }
  while (cell->suiv != NULL) {
    j = cell->j;
    l = cell->suiv->j;
    circ = PlusCourtChemin_apres_c(S, circ, j, l, Tref);
    circ = Ajout_action_apres_c(S, circ, l, 'S', Tref);
    cell = cell->suiv;
  }
  j = cell->j;
  l = C->L->premier->j;
  circ = PlusCourtChemin_apres_c(S, circ, j, l, Tref);
  circ = Ajout_action_apres_c(S, circ, l, 'S', Tref);
  if (C->jmax > *Jdroite) {
    *Jdroite = C->jmax;
  }
}

/*
 * Fonction qui renvoie une liste chainee des circuits
 * du graphe H trouves a l'aide de la procedure <Rech_Circuit>
 */
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
  Rech_Circuit(H, LC);
  return LC;
}

void algorithme_circuit_CasLigne1x1(Grille *G, Solution *S, int graine)
{
  Graphe *H = NULL;
  Lcircuit *LC = NULL;
  Cell_char* *Tref = NULL;
  int Jdroite = 0, JdroiteSav = 0, Drapeau = 0;
  Cell_circuit *circuit = NULL;
  Cell_char *cell = NULL;
  
  H = (Graphe *) calloc(1, sizeof(Graphe));
  if (H == NULL) {
    fprintf(stderr, "Erreur lors de l'allocation du graphe\n");
    return;
  }

  Tref = (Cell_char* *) calloc(G->n, sizeof(Cell_char *));
  if (Tref == NULL) {
    fprintf(stderr, "Erreur lors de l'allocation du tableau Tref\n");
    free(H);
    return;
  }
  
  LC = LC_Initialiser(G, H, Graphe_Rech_Circuit_LC);
  if (LC == NULL) {
    free(H);
    free(Tref);
    return;
  }

  CalculJminJmax(LC);

  circuit = LC->premier;
  while (circuit != NULL) {
    if (Tref[circuit->jmin] == NULL) {
      Drapeau = 1;
      JdroiteSav = Jdroite;
      cell = PlusCourtChemin_apres_c(S, Tref[Jdroite], Jdroite, circuit->jmin, Tref);
      Ajout_action_apres_c(S, cell, circuit->jmin, 'S', Tref);
      if (circuit->jmin > Jdroite) {
	Jdroite = circuit->jmin;
      }
    }
    
    Ajout_circuit_dans_solution(S, circuit, Tref, &Jdroite);

    if (Drapeau == 1) {
      Drapeau = 0;
      PlusCourtChemin_apres_c(S, Tref[circuit->jmin], circuit->jmin, JdroiteSav, Tref);
    }

    circuit = circuit->suiv;
  }
  
  LCDesalloue(LC);
  Graphe_desallocation(H);
  free(Tref);
  
  if (S != NULL) {
    Ecriture_Disque(G->m, G->n, G->nbcoul, graine, S, "Graphe_Graf");
  }
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
  Graphe_desallocation(H);
  
  if (S != NULL) {
    Ecriture_Disque(G->m, G->n, G->nbcoul, graine, S, "Graphe_Naif");
  }
}

/*
 * Procedure qui cherche le circuit dont le debut
 * est le sommet le plus proche de (i,j) et qui met
 * dans <circuit> un pointeur sur le circuit precedent
 */
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


/*
 * Procedure resolvant le probleme du robot trieur a l'aide
 * d'un parcours des circuits dans la grille obtenus par
 * <Rech_Circuit> : le parcours de la grille suit l'enchainement
 * de sommets des circuits mais a la fin de chaque circuit
 * (suppression incluse) on cherche le circuit restant le
 * plus proche de la position courante du robot trieur
 */
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

  free(LC);
  Graphe_desallocation(H);

  if (S != NULL) {
    Ecriture_Disque(G->m, G->n, G->nbcoul, graine, S, nom_algo);
  }
}

void algorithme_ucpc_ameliore(Grille *G, Solution *S, int graine)
{
  algorithme_circuit_plus_proche(G, S, graine, Graphe_Rech_Circuit_LC, "Graphe_Ameliore");
}

void algorithme_general(Grille *G, Solution *S, int graine)
{
  algorithme_circuit_plus_proche(G, S, graine, Graphe_Rech_Circuit_Opt, "Graphe_General");
}

/*
 * Procedure qui cherche le circuit dont le debut
 * est le sommet le plus proche de (i,j) et qui met
 * dans <circuit> un pointeur sur le circuit precedent
 */
CelluleLDC *RecherchePlusProcheCase_coupes(Cell_circuit* *LCtab, int n, CelluleLDC *derniere, CelluleLDC *suiv_pile)
{
  Cell_circuit *cour = NULL, *prec = NULL;
  CelluleLDC *tmp = NULL;
  int z = 0, zmin = -1, i, j, k, l, dist, distMin;

  i = derniere->i;
  j = derniere->j;
  /* la distance entre la derniere case traitee
   * ("reference") et celle qui la suit ("suivante")
   * dans le circuit */
  distMin = abs(i-suiv_pile->i) + abs(j-suiv_pile->j);

  /* traitemet du premier circuit */
  if ((cour = LCtab[z]) != NULL) {
    k = cour->L->premier->i;
    l = cour->L->premier->j;
    /* si ce circuit est plus proche de "reference" que "suivante" */
    if ((dist = abs(i-k) + abs(j-l)) < distMin) {
      zmin = z;
      distMin = dist;
    }
  }
  /* traitement pour le reste de circuits */
  for (z = 1; z < n; ++ z) {
    if ((prec = LCtab[z]) == NULL) {
      continue;
    }
    cour = prec->suiv;
    k = cour->L->premier->i;
    l = cour->L->premier->j;
    /* si ce circuit est plus proche de "reference" que les autres */
    if ((dist = abs(i-k) + abs(j-l)) < distMin) {
      zmin = z;
      distMin = dist;
    }
  }
  /* s'il faut continuer l'algorithme avec un nouveau circuit */
  if (zmin != -1) {
    tmp = LCtab[zmin]->suiv->L->premier;
    LCtab[zmin] = NULL;
    return tmp;
  } /* avec "suivante" sinon */
  else {
    return suiv_pile;
  }
}

void algorithme_general_coupes(Grille *G, Solution *S, int graine)
{
  Graphe *H = NULL;
  Lcircuit *LC = NULL;
  Cell_circuit* *LCtab = NULL;
  Cell_circuit *circuit = NULL, *precedent = NULL;
  Pile p_cell = NULL, p_circuit = NULL;
  CelluleLDC *cell = NULL;
  int ir, jr, n;
  
  H = (Graphe *) calloc(1, sizeof(Graphe));
  if (H == NULL) {
    fprintf(stderr, "Erreur lors de l'allocation du graphe\n");
    return;
  }
  
  LC = LC_Initialiser(G, H, Graphe_Rech_Circuit_Opt);
  if (LC == NULL) {
    free(H);
    return;
  }

  LCtab = (Cell_circuit* *) calloc((n = LCLongueur(LC)), sizeof(Cell_circuit *));
  if (LCtab == NULL) {
    fprintf(stderr, "Erreur lors de l'allocation du tableau de circuits\n");
    LCDesalloue(LC);
    Graphe_desallocation(H);
    return;
  }

  empile(&p_circuit, NULL);
  empile(&p_cell, LC->premier->L->premier);
  while (1) {
    /* aller vers la cellule suivante dans la pile */
    cell = (CelluleLDC *) depile(&p_cell);
    echangerCouleur(G, S, cell->i, cell->j);

    /* si c'est l'avant-derniere cellule du circuit */
    if (cell->suiv == NULL) {
      precedent = (Cell_circuit *) depile(&p_circuit);
      if (precedent == NULL) {
	circuit = LC->premier;
      } else {
	circuit = precedent->suiv;
      }
      /* on le ferme */
      ir = circuit->L->premier->i;
      jr = circuit->L->premier->j;
      echangerCouleur(G, S, ir, jr);
      LCEnleverCelluleSuivante(LC, precedent);

      /* si on a referme tous les circuits commences depuis le debut de l'algorithme */
      if (estPileVide(p_circuit)) {
	/* plus precisement, si on a referme tous les circuits */
	if (LCVide(LC)) {
	  break;
	}
	/* on en commence un nouveau sinon */
	RecherchePlusProcheCircuit(LC, ir, jr, &precedent);
	if (precedent == NULL) {
	  circuit = LC->premier;
	} else {
	  circuit = precedent->suiv;
	}
	empile(&p_circuit, precedent);
	empile(&p_cell, circuit->L->premier);
      }
    } /* s'il y a encore des cellules a traiter dans le circuit courant */
    else {
      empile(&p_cell, RecherchePlusProcheCase_coupes(LCtab, n, cell, cell->suiv));
    } 
  }
    
  free(LC);
  Graphe_desallocation(H);
  free(LCtab);

  if (S != NULL) {
    Ecriture_Disque(G->m, G->n, G->nbcoul, graine, S, "Graphe_Coupes");
  }
}
