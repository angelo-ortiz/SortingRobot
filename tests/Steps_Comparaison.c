#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TMAX 256
#define NB_ALGO 2

void obtenir_rapport_donnees(FILE *lecture[], FILE *ecriture[])
{
  int i, n;
  double pas[NB_ALGO +1];
  char buffer[NB_ALGO + 1][TMAX];
  while (fgets(*buffer, TMAX, *lecture) != NULL) {
    sscanf(*buffer, " %d %lf\n", &n, pas);
    for (i = 1; i < NB_ALGO + 1; ++i) {
      fgets(buffer[i], TMAX, lecture[i]);
      sscanf(buffer[i], " %d %lf\n", &n, pas+i);
    }
    for (i = 0; i < NB_ALGO; ++i) {
      fprintf(ecriture[i], "%d %7.6e\n", n, pas[i+1] / pas[0]);
    }
  }	
}

int main (void) 
{
  int i;
  char *res_base[NB_ALGO + 1] = {"../data/vector/ucpc.txt", "../data/vector/avl.txt", \
				 "../data/vector/g_coupes.txt"};
  char *res_rap[NB_ALGO] = {"../data/vector/ucpc_avl.txt", "../data/vector/ucpc_coupes.txt"};
  FILE *in_files[NB_ALGO + 1], *out_files[NB_ALGO];

  for (i = 0; i < NB_ALGO + 1; ++i) {
    in_files[i] = fopen(res_base[i], "r");
    if (in_files[i] == NULL) exit(1);
  }

  for (i = 0; i < NB_ALGO; ++i) {
    out_files[i] = fopen(res_rap[i], "w");
    if (out_files[i] == NULL) exit(1);
  }

  obtenir_rapport_donnees(in_files, out_files);

  for (i = 0; i < NB_ALGO + 1; ++i) {
    fclose(in_files[i]);
  }

  for (i = 0; i < NB_ALGO; ++i) {
    fclose(out_files[i]);
  }
  return 0;
}
