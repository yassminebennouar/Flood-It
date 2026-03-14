#ifndef SEQUENCE_ALEA_RECURSIVE_H
#define SEQUENCE_ALEA_RECURSIVE_H

#include "Liste_case.h"
#include "API_Grille.h"

void trouve_zone_rec(int **M, int dim, int i, int j,
                  int *taille, ListeCase *L, int couleur_cible);

int grille_est_unie(int **M, int dim);

int sequence_aleatoire_rec(int **M, Grille *G, int dim, int nbcl, int aff);

#endif

