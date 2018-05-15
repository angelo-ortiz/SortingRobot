#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TMAX 256
#define NB_ALGO 6

void lineariser_donnees(FILE *lecture, FILE *ecriture)
{
  int n;
  double pas, pas_lin;
  char buffer[TMAX];
  while (fgets(buffer, TMAX, lecture) != NULL) {
    sscanf(buffer, " %d %lf\n", &n, &pas);
    pas_lin = pas/1000.;
    fprintf(ecriture, "%d %7.6e\n", n, pas_lin);
  }	
}

int main (void) 
{
  int i;
  char *res_base[NB_ALGO] = {"../data/vector/avl.txt", "../data/vector/ucpc.txt", \
			     "../data/vector/g_naif.txt", "../data/vector/g_ameliore.txt", \
			     "../data/vector/g_general.txt", "../data/vector/g_coupes.txt"};
  char *res_lin[NB_ALGO] = {"../data/vector/avl_lin.txt", "../data/vector/ucpc_lin.txt", \
			     "../data/vector/g_naif_lin.txt", "../data/vector/g_ameliore_lin.txt", \
			     "../data/vector/g_general_lin.txt", "../data/vector/g_coupes_lin.txt"};
  FILE *in_files[NB_ALGO], *out_files[NB_ALGO];

  for (i = 0; i < NB_ALGO; ++i) {
    in_files[i] = fopen(res_base[i], "r");
    if (in_files[i] == NULL) exit(1);
    out_files[i] = fopen(res_lin[i], "w");
    if (out_files[i] == NULL) exit(1);

    lineariser_donnees(in_files[i], out_files[i]);
    fclose(in_files[i]);
    fclose(out_files[i]);
  }
  
  return 0;
}
