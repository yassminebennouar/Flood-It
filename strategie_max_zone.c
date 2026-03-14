#include "strategie_max_zone.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/* Fonction pour explorer une zone */
static void explorer_zone(int **M, int **visite, int dim, int i, int j,
                         int couleur, ListeCase *zone, int *taille) {
    if (i < 0 || i >= dim || j < 0 || j >= dim) return;
    if (visite[i][j] || M[i][j] != couleur) return;
    
    visite[i][j] = 1;
    ajoute_en_tete(zone, i, j);
    (*taille)++;
    
    explorer_zone(M, visite, dim, i-1, j, couleur, zone, taille);
    explorer_zone(M, visite, dim, i+1, j, couleur, zone, taille);
    explorer_zone(M, visite, dim, i, j-1, couleur, zone, taille);
    explorer_zone(M, visite, dim, i, j+1, couleur, zone, taille);
}

/* Initialiser */
void init_bordure_zone(BordureZone *bz, int nbcl) {
    bz->zones = NULL;
    bz->nbcl = nbcl;
    bz->compteurs = (int *)calloc(nbcl, sizeof(int));
}

/* Explorer récursivement depuis une case */
static void explorer_recursif(int **M, int **visite, int dim, int i, int j,
                             int couleur, ListeCase *zone, int *taille) {
    if (i < 0 || i >= dim || j < 0 || j >= dim) return;
    if (visite[i][j] || M[i][j] != couleur) return;
    
    visite[i][j] = 1;
    ajoute_en_tete(zone, i, j);
    (*taille)++;
    
    explorer_recursif(M, visite, dim, i-1, j, couleur, zone, taille);
    explorer_recursif(M, visite, dim, i+1, j, couleur, zone, taille);
    explorer_recursif(M, visite, dim, i, j-1, couleur, zone, taille);
    explorer_recursif(M, visite, dim, i, j+1, couleur, zone, taille);
}

/* Trouver toutes les zones adjacentes à la ZSG */
void calculer_zones_adjacentes(int **M, int dim, BordureZone *bz) {
    /* Reset */
    bz->zones = NULL;
    for (int i = 0; i < bz->nbcl; i++) bz->compteurs[i] = 0;
    
    /* Tableau de visite */
    int **visite = (int **)calloc(dim, sizeof(int *));
    for (int i = 0; i < dim; i++) {
        visite[i] = (int *)calloc(dim, sizeof(int));
    }
    
    /* Marquer la ZSG (tout ce qui a la couleur de M[0][0]) */
    int couleur_zsg = M[0][0];
    ListeCase zsg = NULL;
    int taille_zsg = 0;
    explorer_recursif(M, visite, dim, 0, 0, couleur_zsg, &zsg, &taille_zsg);
    
    /* Explorer autour de la ZSG */
    ListeCase cur_zsg = zsg;
    while (cur_zsg != NULL) {
        int i = cur_zsg->i;
        int j = cur_zsg->j;
        
        /* 4 directions */
        int dirs[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
        for (int d = 0; d < 4; d++) {
            int ni = i + dirs[d][0];
            int nj = j + dirs[d][1];
            
            if (ni >= 0 && ni < dim && nj >= 0 && nj < dim) {
                if (!visite[ni][nj] && M[ni][nj] != couleur_zsg) {
                    /* Nouvelle zone trouvée */
                    ListeCase nouvelle_zone = NULL;
                    int taille_zone = 0;
                    int couleur_zone = M[ni][nj];
                    
                    explorer_recursif(M, visite, dim, ni, nj, couleur_zone, &nouvelle_zone, &taille_zone);
                    
                    if (taille_zone > 0) {
                        /* Créer la zone */
                        ZoneComplete *zone = (ZoneComplete *)malloc(sizeof(ZoneComplete));
                        zone->couleur = couleur_zone;
                        zone->taille = taille_zone;
                        zone->cases = nouvelle_zone;
                        zone->suiv = bz->zones;
                        bz->zones = zone;
                        
                        /* Ajouter au compteur */
                        bz->compteurs[couleur_zone] += taille_zone;
                    }
                }
            }
        }
        cur_zsg = cur_zsg->suiv;
    }
    
    /* Nettoyer */
    detruit_liste(&zsg);
    for (int i = 0; i < dim; i++) free(visite[i]);
    free(visite);
}

/* Choisir la meilleure couleur */
int choisir_couleur_max_zone(BordureZone *bz, int couleur_zsg) {
    int meilleure = -1;
    int max = 0;
    
    for (int c = 0; c < bz->nbcl; c++) {
        if (c != couleur_zsg && bz->compteurs[c] > max) {
            max = bz->compteurs[c];
            meilleure = c;
        }
    }
    
    return meilleure;
}

/* Mettre à jour après changement */
void mettre_a_jour_zone(int **M, int dim, BordureZone *bz, int couleur_choisie) {
    /* Reset compteurs */
    for (int i = 0; i < bz->nbcl; i++) bz->compteurs[i] = 0;
    
    /* Parcourir zones */
    ZoneComplete **prev = &(bz->zones);
    ZoneComplete *cur = bz->zones;
    
    while (cur != NULL) {
        if (cur->couleur == couleur_choisie) {
            /* Supprimer cette zone */
            ZoneComplete *a_supp = cur;
            *prev = cur->suiv;
            cur = cur->suiv;
            
            detruit_liste(&(a_supp->cases));
            free(a_supp);
        } else {
            /* Garder et compter */
            bz->compteurs[cur->couleur] += cur->taille;
            prev = &(cur->suiv);
            cur = cur->suiv;
        }
    }
}

/* Libérer mémoire */
void free_bordure_zone(BordureZone *bz) {
    free(bz->compteurs);
    
    ZoneComplete *cur = bz->zones;
    while (cur != NULL) {
        ZoneComplete *suiv = cur->suiv;
        detruit_liste(&(cur->cases));
        free(cur);
        cur = suiv;
    }
    bz->zones = NULL;
}

/* ========== STRATÉGIE PRINCIPALE ========== */

/* Vérifier si grille unie */
static int grille_unie(int **M, int dim) {
    int couleur = M[0][0];
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            if (M[i][j] != couleur) return 0;
        }
    }
    return 1;
}

/* Changer couleur de la ZSG  */
static void changer_couleur_zsg_simple(int **M, int dim, int nouvelle_couleur) {
    int ancienne = M[0][0];
    int **visite = (int **)calloc(dim, sizeof(int *));
    for (int i = 0; i < dim; i++) {
        visite[i] = (int *)calloc(dim, sizeof(int));
    }
    
    ListeCase zone = NULL;
    int taille = 0;
    explorer_zone(M, visite, dim, 0, 0, ancienne, &zone, &taille);
    
    ListeCase cur = zone;
    while (cur != NULL) {
        M[cur->i][cur->j] = nouvelle_couleur;
        cur = cur->suiv;
    }
    
    detruit_liste(&zone);
    for (int i = 0; i < dim; i++) free(visite[i]);
    free(visite);
}

/* Stratégie max-bordure-zone */
int strategie_max_bordure_zone(int **M, Grille *G, int dim, int nbcl, int aff) {
    int iterations = 0;
    srand(time(NULL));
    
    /* Initialiser bordure-zone */
    BordureZone bz;
    init_bordure_zone(&bz, nbcl);
    
    while (!grille_unie(M, dim)) {
        int couleur_zsg = M[0][0];
        
        /* Calculer zones adjacentes */
        calculer_zones_adjacentes(M, dim, &bz);
        
        /* Choisir meilleure couleur */
        int meilleure = choisir_couleur_max_zone(&bz, couleur_zsg);
        
        if (meilleure == -1) {
            /* Choisir aléatoirement */
            do {
                meilleure = rand() % nbcl;
            } while (meilleure == couleur_zsg);
        }
        
        /* Appliquer le changement */
        changer_couleur_zsg_simple(M, dim, meilleure);
        
        /* Mettre à jour */
        mettre_a_jour_zone(M, dim, &bz, meilleure);
        
        /* Affichage */
        if (aff == 1 && G != NULL) {
            for (int i = 0; i < dim; i++) {
                for (int j = 0; j < dim; j++) {
                    Grille_attribue_couleur_case(G, i, j, M[i][j]);
                }
            }
            Grille_redessine_Grille(G);
            Grille_attente_touche();
        }
        
        iterations++;
        
        /* Sécurité */
        if (iterations > dim * dim * 10) {
            printf("ATTENTION: Limite atteinte (%d iterations)\n", iterations);
            break;
        }
    }
    
    free_bordure_zone(&bz);
    return iterations;
}

