#include "../include/src/AVL.h"

void hauteurFils(AVL *avl, int* hg, int* hd)
{
  /* Si les fils sont des arbres vides */
  *hg = -1;
  *hd = -1;
  
  if (avl->fg != NULL) {
    *hg = avl->fg->hauteur;
  }
  if (avl->fd != NULL) {
    *hd = avl->fd->hauteur;
  }
}

void majHauteur(AVL *avl)
{
  int hg, hd;
  /* Si l'arbre est vide */
  if (avl == NULL) {
    return;
  }
  hauteurFils(avl, &hg, &hd);
  avl->hauteur = 1 + (hg > hd ? hg : hd);
}

AVL *creerNoeud(int content, AVL *fg, AVL *fd)
{
  AVL *noeud = (AVL *) calloc(1, sizeof(AVL));
  if (noeud == NULL) {
    fprintf(stderr, "Erreur lors de l'allocation d'un noeud\n");
    return NULL;
  }
  noeud->content = content;
  noeud->fg = fg;
  noeud->fd = fd;
  majHauteur(noeud);
  return noeud;
}

AVL *rotDroite(AVL *root)
{
  AVL *nRoot = root->fg;
  root->fg = nRoot->fd;
  nRoot->fd = root;
  majHauteur(root);
  majHauteur(nRoot);
  return nRoot;
}

AVL *rotGauche(AVL *root)
{
  AVL *nRoot = root->fd;
  root->fd = nRoot->fg;
  nRoot->fg = root;
  majHauteur(root);
  majHauteur(nRoot);
  return nRoot;
}

AVL *doubleRotDroite(AVL *root)
{
  root->fg = rotGauche(root->fg);
  majHauteur(root);
  return rotDroite(root);
}

AVL *equilibrer(AVL *avl)
{
  int HG, HD, hg, hd;
  /* Si l'arbre est vide */
  if (avl == NULL) {
    return NULL;
  }
  hauteurFils(avl, &HG, &HD);
  if (HG-HD == 2) {
    hauteurFils(avl->fg, &hg, &hd);
    if (hg < hd) {
      avl->fg = rotGauche(avl->fg);
    }
    avl = rotDroite(avl);
  }
  if (HG-HD == -2) {
    hauteurFils(avl->fd, &hg, &hd);
    if (hg > hd) {
      avl->fd = rotDroite(avl->fd);
    }
    avl = rotGauche(avl);
  }
  return avl;
}

AVL *insererNoeud(AVL *avl, int val)
{
  if (avl == NULL) {
    return creerNoeud(val, NULL, NULL);
  }
  /* Si la valeur est deja dans l'arbre, on fait rien */
  if (avl->content == val) {
    return avl;
  }
  /* On ajoute la valeur dans le fils gauche si la
   * valeur est plus petite que celle de la racine */
  if (avl->content > val) {
    avl->fg = insererNoeud(avl->fg, val);
  } /* on l'ajoute dans le fils droit sinon */
  else {
    avl->fd = insererNoeud(avl->fd, val);
  }
  majHauteur(avl);
  avl = equilibrer(avl);
  return avl;
}

AVL *supprimerMax(AVL *avl, int *valMax)
{
  AVL *tmp = NULL;
  if (avl == NULL) {
    return avl;
  }
  /* S'il n'y a plus de fils droit,
   * i.e. c'est le noeud de la valeur maximum */
  if (avl->fd == NULL) {
    *valMax = avl->content;
    tmp = avl->fg;
    free(avl);
    return tmp;
  } else {
    avl->fd = supprimerMax(avl->fd, valMax);
    return avl;
  }
}

AVL *enleverNoeud(AVL *avl, int val)
{
  AVL *tmp = NULL;
  int valMax = -1;
  if (avl == NULL) {
    return avl;
  }
  /* Si la valeur est dans le sous-arbre gauche */
  if (avl->content > val) {
    avl->fg = enleverNoeud(avl->fg, val);;
  } /* Si la valeur est dans le sous-arbre droit */
  else if (avl->content < val) {
    avl->fd = enleverNoeud(avl->fd, val);
  } /* Si la valeur est dans la racine */
  else {
    /* S'il n'y a pas de fils gauche,
     * on ne garde que le fils droit */
    if (avl->fg == NULL) {
      tmp = avl->fd;
      free(avl);
      avl = tmp;
    } else {
      avl->fg = supprimerMax(avl->fg, &valMax);
      avl->content = valMax;
    }
  }
  majHauteur(avl);
  avl = equilibrer(avl);
  return avl;
}

void AVLafficher(AVL *avl)
{
  if (avl == NULL) {
    return;
  }
  AVLafficher(avl->fg);
  printf("val=%d\th=%d\n", avl->content, avl->hauteur);
  AVLafficher(avl->fd);
}

void AVLdesalloue(AVL *avl)
{
  /* Si l'arbre est vide */
  if (avl == NULL) {
    return;
  }
  AVLdesalloue(avl->fg);
  AVLdesalloue(avl->fd);
  free(avl);
}

int AVLrechercherPlusProcheCase(AVL *avl, int l)
{
  int j, jpp, distMin = 9999999, diff;
  if (avl == NULL) {
    return -1;
  }
  while (avl != NULL) {
    j = avl->content;
    diff = l - j;
    if (diff == 0) {
      /* S'ils sont dans la meme colonne */
      return j;
    } else if (diff > 0) {
      /* Si (i,j) est a gauche de (k,l) */
      if (diff <= distMin) {
	/* On ajoute l'egalite car on cherche
	 * la case la plus a gauche */
        distMin = diff;
        jpp = j;
      }
      avl = avl->fd;
    }  else {
      /* Si (i,j) est a droite de (k,l) */
      if (-diff < distMin) {
        distMin = -diff;
        jpp = j;
      }
      avl = avl->fg;
    }
  }
  return jpp;
}
