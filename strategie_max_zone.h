#ifndef STRATEGIES_MAX_ZONE_H
#define STRATEGIES_MAX_ZONE_H

#include "Liste_case.h"
#include "API_Grille.h"

/* Structure pour une zone complète */
typedef struct ZoneComplete {
    int couleur;
    int taille;
    ListeCase cases;
    struct ZoneComplete *suiv;
} ZoneComplete;

/* Structure pour la bordure-zone */
typedef struct {
    ZoneComplete *zones;
    int *compteurs;
    int nbcl;
} BordureZone;

/* Fonctions Exercice 4 */
void init_bordure_zone(BordureZone *bz, int nbcl);
void calculer_zones_adjacentes(int **M, int dim, BordureZone *bz);
int choisir_couleur_max_zone(BordureZone *bz, int couleur_zsg);
void mettre_a_jour_zone(int **M, int dim, BordureZone *bz, int couleur_choisie);
void free_bordure_zone(BordureZone *bz);

int strategie_max_bordure_zone(int **M, Grille *G, int dim, int nbcl, int aff);

#endif
