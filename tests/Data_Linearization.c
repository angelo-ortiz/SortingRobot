#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TMAX 256
#define NB_APP 4

double complexite_naif(int n)
{
  double n_double = (double) n;
  return n_double * n_double * n_double * n_double;
}

double complexite_circulaire(int n)
{
  double n_double = (double) n;
  return n_double * n_double * n_double * n_double;
}

double complexite_parcouleur(int n)
{
  double n_double = (double) n;
  /*
   * Il y a n² cases et le nombre de couleurs est n/2, 
   * et on sait que alpha = plafond(nb_cases/nb_couleurs).
   */
  double alpha = ceil(n_double*n_double/((double) (n/2)));
  return n_double * n_double * alpha;
}

double complexite_paravl(int n)
{
  double n_double = (double) n;
  double alpha = ceil(n_double*n_double/((double) (n/2)));
  double min = (n_double < alpha) ? n_double : alpha;
  return n_double * n_double * n_double * log(min);
}

/*
 * Pointeur sur fonction, ici la complexite d'une version
 * de l'algorithme au plus proche
 */
typedef double (*p_complexite) (int n);
p_complexite complexites[NB_APP] = {complexite_naif, complexite_circulaire, \
			       complexite_parcouleur, complexite_paravl};

void lineariser_donnees(FILE *lecture, FILE *ecriture, p_complexite complexite)
{
  int n;
  double temps, temps_lin;
  char buffer[TMAX];
  while (fgets(buffer, TMAX, lecture) != NULL) {
    sscanf(buffer, " %d %lf\n", &n, &temps);
    temps_lin = temps/complexite(n);
    fprintf(ecriture, "%d %7.6e\n", n, temps_lin);
  }	
}

int main (void) 
{
  int i;
  char *res_base[NB_APP] = {"../data/size/naif.txt", "../data/size/circulaire.txt", \
			     "../data/size/couleur.txt", "../data/size/avl.txt"};
  char *res_lin[NB_APP] = {"../data/size/naif_lin.txt", "../data/size/circulaire_lin.txt", \
			    "../data/size/couleur_lin.txt", "../data/size/avl_lin.txt"};
  FILE *in_files[NB_APP], *out_files[NB_APP];

  for (i = 0; i < NB_APP; ++i) {
    in_files[i] = fopen(res_base[i], "r");
    if (in_files[i] == NULL) exit(1);
    out_files[i] = fopen(res_lin[i], "w");
    if (out_files[i] == NULL) exit(1);

    lineariser_donnees(in_files[i], out_files[i], complexites[i]);
    fclose(in_files[i]);
    fclose(out_files[i]);
  }
  
  return 0;
}
