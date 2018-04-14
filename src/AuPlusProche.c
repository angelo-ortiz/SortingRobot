#include "../include/src/AuPlusProche.h"

sr_algorithme app_algorithmes[NB_APP] = {algorithme_naif, algorithme_circulaire, \
					 algorithme_parcouleur, algorithme_paravl};

int couleurPieceRobot(Grille *G)
{
  return G->T[G->ir][G->jr].robot;
}

int robotPortePiece(Grille *G)
{
  return couleurPieceRobot(G) != -1;
}

int estCaseNoire(Grille *G, int i, int j)
{
  return G->T[i][j].fond == G->T[i][j].piece;
}

int estPieceNoire(Grille *G, int i, int j)
{
  return G->T[i][j].piece == -1;
}

int estCaseCouleur_c(Grille *G, int i, int j, int c)
{
  return !estCaseNoire(G, i, j) && G->T[i][j].fond == c;
}

int estCaseNonNoire(Grille *G, int i, int j)
{
  return !estCaseNoire(G, i, j) && !estPieceNoire(G, i, j);
}

void PlusCourtChemin(Solution *S, int i, int j, int k, int l)
{
  int h = j, v = i, pasH = 1, pasV = 1;
  char lH = 'R', lV = 'D';
  if (k < i) {
    pasV = -1;
    lV = 'U';
  }
  if (l < j) {
    pasH = -1;
    lH = 'L';
  }
  
  for (h = j; h != l; h += pasH) {
    if (S != NULL) {
      Ajout_action(S, lH);
    }
  }
  for (v = i; v != k; v += pasV) {
    if (S != NULL) {
      Ajout_action(S, lV);
    }
  }
}

void RechercheCaseNaif_c(Grille *G, int c, int i, int j, int *k, int *l)
{
  int h, v, m = G->m, n = G->n, distMin = m + n, dist;
  for (v = 0; v < m; v++) {
    for (h = 0; h < n; h++) {
      if (estCaseCouleur_c(G, v, h, c) && ((dist = abs(v-i) + abs(h-j)) < distMin)) {
	*k = v;
	*l = h;
	distMin = dist;
      }
    }
  }
}

void RechercheCaseNaif_nn(Grille *G, int i, int j, int *k, int *l)
{
  int h, v, m = G->m, n = G->n, distMin = m + n, dist;
  for (v = 0; v < m; v++) {
    for (h = 0; h < n; h++) {
      if (estCaseNonNoire(G, v, h) && ((dist = abs(v-i) + abs(h-j)) < distMin)) {
	*k = v;
	*l = h;
	distMin = dist;
      }
    }
  }
}

void echangerCouleur(Grille *G, Solution *S, int k, int l)
{
  PlusCourtChemin(S, G->ir, G->jr, k, l);
  changement_case(G, k, l);
  swap_case(G);
  if (S != NULL) {
    Ajout_action(S, 'S');
  }
}

void algorithme_naif(Grille *G, Solution *S, int graine)
{
  int k, l, c, m = G->m, n = G->n, nbCases = m * n;
  while (nbCases != G->cptr_noire) {
    if (couleurPieceRobot(G) == -2) {
      fprintf(stderr, "Erreur : Le robot n'est pas dans la case ou il est cense etre\n");
      return;
    }
    if (!robotPortePiece(G)) {
      RechercheCaseNaif_nn(G, G->ir, G->jr, &k, &l);
      echangerCouleur(G, S, k, l);
    }
    c = couleurPieceRobot(G);
    RechercheCaseNaif_c(G, c, G->ir, G->jr, &k, &l);
    echangerCouleur(G, S, k, l);
  }
  if (S != NULL) {
    Ecriture_Disque(G->m, G->n, G->nbcoul, graine, S, "Naif");
  }
}

/*
 * Procedure qui fait pointer les trois premiers arguments
 * vers les valeurs des trois derniers
 */
void initialiserVariables(int *v, int *hg, int *hd, int val_v, int val_hg, int val_hd)
{
  *v = val_v;
  *hg = val_hg;
  *hd = val_hd;
}

void RechercheCaseCirculaire_c(Grille *G, int c, int i, int j, int *k, int *l)
{
  int hg, hd, v, m = G->m, n = G->n, L = 1;
  while (1) {
    initialiserVariables(&v, &hg, &hd, i - L, j, j);
    while (v <= i) {
      if (v >= 0 && hg >= 0 && estCaseCouleur_c(G, v, hg, c)) {
	*k = v;
	*l = hg;
	return;
      } else if (v >= 0 && hd < n && estCaseCouleur_c(G, v, hd, c)) {
	*k = v;
	*l = hd;
	return;
      } else {
	v ++;
	hg --;
	hd ++;
      }
    }
    initialiserVariables(&v, &hg, &hd, i + 1, j - L + 1, j + L - 1);
    while (v <= i + L) {
      if (v < m && hg >= 0 && estCaseCouleur_c(G, v, hg, c)) {
	*k = v;
	*l = hg;
	return;
      } else if (v < m && hd < n && estCaseCouleur_c(G, v, hd, c)) {
	*k = v;
	*l = hd;
	return;
      } else {
	v ++;
	hg ++;
	hd --;
      }
    }
    L ++;
  }
}

void RechercheCaseCirculaire_nn(Grille *G, int i, int j, int *k, int *l)
{
  int hg, hd, v, m = G->m, n = G->n, L = 1;
  if (estCaseNonNoire(G, i, j)) {
    *k = i;
    *l = j;
    return;
  }
  while (1) {
    initialiserVariables(&v, &hg, &hd, i - L, j, j);
    while (v <= i) {
      if (v >= 0 && hg >= 0 && estCaseNonNoire(G, v, hg)) {
        *k = v;
	*l = hg;
	return;
      } else if (v >= 0 && hd < n && estCaseNonNoire(G, v, hd)) {
        *k = v;
	*l = hd;
	return;
      } else {
	v ++;
	hg --;
	hd ++;
      }
    }
    initialiserVariables(&v, &hg, &hd, i + 1, j - L + 1, j + L - 1);
    while (v <= i + L) {
      if (v < m && hg >= 0 && estCaseNonNoire(G, v, hg)) {
	*k = v;
	*l = hg;
	return;
      } else if (v < m && hd < n && estCaseNonNoire(G, v, hd)) {
	*k = v;
	*l = hd;
	return;
      } else {
	v ++;
	hg ++;
	hd --;
      }
    }
    L ++;
  }
}

void algorithme_circulaire(Grille *G, Solution *S, int graine)
{
  int k, l, c, m = G->m, n = G->n, nbCases = m * n;
  while (nbCases != G->cptr_noire) {
    if (couleurPieceRobot(G) == -2) {
      fprintf(stderr, "Erreur : Le robot n'est pas dans la case ou il est cense etre\n");
      return;
    }
    if (!robotPortePiece(G)) {
      RechercheCaseCirculaire_nn(G, G->ir, G->jr, &k, &l);
      echangerCouleur(G, S, k, l);
    }
    c = couleurPieceRobot(G);
    RechercheCaseCirculaire_c(G, c, G->ir, G->jr, &k, &l);
    echangerCouleur(G, S, k, l);
  }
  if (S != NULL) {
    Ecriture_Disque(G->m, G->n, G->nbcoul, graine, S, "Circulaire");
  }
}

/*
 * Fonction qui initialise un tableau de listes chainees
 * doublement chainees correspondant a une couleur distincte
 * chacune
 */
LDC *initialiserLDC(Grille *G)
{
  int i, j;
  LDC *TC = (LDC *) calloc(G->nbcoul, sizeof(LDC));
  if (TC == NULL) {
    fprintf(stderr, "Erreur lors de l'allocation du tableau de LDC\n");
    return NULL;
  }
  for (i = 0; i < G->nbcoul; ++i) {
    LDCInitialise(TC + i);
  }
  for (i = 0; i < G->m; ++i) {
    for (j = 0; j < G->n; ++j) {
      if (estCaseNoire(G, i, j)) continue;
      LDCInsererEnFin(TC + G->T[i][j].fond, i, j);
    }
  }
  return TC;
}

void algorithme_parcouleur(Grille *G, Solution *S, int graine)
{
  int k, l, c, m = G->m, n = G->n, nbCases = m * n;
  LDC *TC = initialiserLDC(G);
  CelluleLDC *recherche = NULL;
  if (TC == NULL) {
    return;
  }
  while (nbCases != G->cptr_noire) {
    if (couleurPieceRobot(G) == -2) {
      fprintf(stderr, "Erreur : Le robot n'est pas dans la case ou il est cense etre\n");
      return;
    }
    if (!robotPortePiece(G)) {
      RechercheCaseCirculaire_nn(G, G->ir, G->jr, &k, &l);
      echangerCouleur(G, S, k, l);
    }
    c = couleurPieceRobot(G);
    recherche = LDCrechercherPlusProcheCase(TC + c, G->ir, G->jr);
    if (recherche == NULL) {
      fprintf(stderr, "Erreur : Il n'y a pas de case avec la couleur cherchee\n");
      return;
    }
    echangerCouleur(G, S, recherche->i, recherche->j);
    LDCenleverCellule(TC + c, recherche);
  }
  free(TC);
  if (S != NULL) {
    Ecriture_Disque(G->m, G->n, G->nbcoul, graine, S, "Couleur");
  }
}

void RechercheCaseAVL_c(Grille *G, int c, AVL** *M, int i, int j, int *k, int *l)
{
  int h, v, m = G->m, n = G->n, distMin = m + n, dist;
  for (v = 0; v < m; v++) {
    h = AVLrechercherPlusProcheCase(M[c][v], j);
    if (h < 0) {
      continue;
    }
    if ((dist = abs(v-i) + abs(h-j)) < distMin) {
      *k = v;
      *l = h;
      distMin = dist;
    }
  }
}

/*
 * Fonction qui initialise une matrice d'arbres AVL
 * dont chaque ligne correspond a une couleur distincte
 * et chaque colonnne correspond a une ligne de la
 * grille, i.e. chaque arbre contient les cases d'une
 * meme ligne ayant toutes la meme couleur
 */
AVL** *initialiserAVL(Grille *G)
{
  int i, j;
  AVL** *M = (AVL** *) calloc(G->nbcoul, sizeof(AVL* *));
  if (M == NULL) {
    fprintf(stderr, "Erreur lors de l'allocation de la matrice d'AVL\n");
    return NULL;
  }
  for (i = 0; i < G->nbcoul; ++i) {
    M[i] = (AVL* *) calloc(G->m, sizeof(AVL *));
    if (M[i] == NULL) {
      for (j = 0; j < i; ++j) {
	free(M[j]);
      }
      free(M);
      fprintf(stderr, "Erreur lors de l'allocation d'un ligne d'AVL\n");
      return NULL;
    }
  }
  for (i = 0; i < G->m; ++i) {
    for (j = 0; j < G->n; ++j) {
      if (estCaseNoire(G, i, j)) continue;
      M[G->T[i][j].fond][i] = insererNoeud(M[G->T[i][j].fond][i], j);
    }
  }
  return M;
}

void algorithme_paravl(Grille *G, Solution *S, int graine)
{
  int i, k, l, c, m = G->m, n = G->n, nbCases = m * n;
  AVL** *M = initialiserAVL(G);
  if (M == NULL) {
    return;
  }
  while (nbCases != G->cptr_noire) {
    if (couleurPieceRobot(G) == -2) {
      fprintf(stderr, "Erreur : Le robot n'est pas dans la case ou il est cense etre\n");
      return;
    }
    if (!robotPortePiece(G)) {
      RechercheCaseCirculaire_nn(G, G->ir, G->jr, &k, &l);
      echangerCouleur(G, S, k, l);
    }
    c = couleurPieceRobot(G);
    RechercheCaseAVL_c(G, c, M, G->ir, G->jr, &k, &l);
    echangerCouleur(G, S, k, l);
    M[c][k] = enleverNoeud(M[c][k],l);
  }
  for (i = 0; i < G->nbcoul; ++i) {
    free(M[i]);
  }
  free(M);
  if (S != NULL) {
    Ecriture_Disque(G->m, G->n, G->nbcoul, graine, S, "AVL");
  }
}
