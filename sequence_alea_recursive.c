#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "Liste_case.h"
#include "sequence_alea_recursive.h"
/* Fonction récursive pour trouver la ZSG */
void trouve_zone_rec(int **M, int dim, int i, int j, 
                     int *taille, ListeCase *L, int couleur_cible) {
    
    /* Si hors de la grille */
    if (i < 0 || i >= dim || j < 0 || j >= dim) {
        return;
    }
    
    /* Si case déjà visitée (marquée négative) */
    if (M[i][j] < 0) {
        return;
    }
    
    /* Si pas la bonne couleur */
    if (M[i][j] != couleur_cible) {
        return;
    }
    
    /* Marquer comme visitée en mettant en négatif */
    M[i][j] = -M[i][j] - 1;
    
    /* Ajouter à la liste */
    ajoute_en_tete(L, i, j);
    (*taille)++;
    
    /* Explorer les 4 directions */
    trouve_zone_rec(M, dim, i-1, j, taille, L, couleur_cible); /* haut */
    trouve_zone_rec(M, dim, i+1, j, taille, L, couleur_cible); /* bas */
    trouve_zone_rec(M, dim, i, j-1, taille, L, couleur_cible); /* gauche */
    trouve_zone_rec(M, dim, i, j+1, taille, L, couleur_cible); /* droite */
}

int grille_uniforme(int **M, int dim) {
    int couleur_ref = M[0][0];
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            if (M[i][j] != couleur_ref) {
                return 0;
            }
        }
    }
    return 1;
}


int sequence_aleatoire_rec(int **M, Grille *G, int dim, int nbcl, int aff) {
    int iterations = 0;
    
    if (nbcl <= 1) {
        printf("ERREUR: Il faut au moins 2 couleurs\n");
        return 0;
    }
    
    srand(time(NULL));
    
    while (!grille_uniforme(M, dim)) {
        /* 1. Trouver la ZSG */
        ListeCase zone = NULL;
        int taille_zone = 0;
        int couleur_actuelle = M[0][0];
        
        trouve_zone_rec(M, dim, 0, 0, &taille_zone, &zone, couleur_actuelle);
        
        /* 2. Choisir nouvelle couleur différente */
        int nouvelle_couleur;
        do {
            nouvelle_couleur = rand() % nbcl;
        } while (nouvelle_couleur == couleur_actuelle);
        
        /* 3. Changer la couleur des cases de la zone ET restaurer en même temps */
        ListeCase courant = zone;
        while (courant != NULL) {
            int i = courant->i;
            int j = courant->j;
            /* Restaurer d'abord si c'était marqué négatif */
            if (M[i][j] < 0) {
                M[i][j] = -(M[i][j] + 1);  /* Restauration */
            }
            /* Puis mettre la nouvelle couleur */
            M[i][j] = nouvelle_couleur;
            courant = courant->suiv;
        }
        
        /* 4. Mettre à jour l'affichage */
        if (aff == 1) {
            for (int i = 0; i < dim; i++) {
                for (int j = 0; j < dim; j++) {
                    Grille_attribue_couleur_case(G, i, j, M[i][j]);
                }
            }
            Grille_redessine_Grille(G);
            Grille_attente_touche();
        }
        
        /* 5. Libérer la liste */
        detruit_liste(&zone);
        
        iterations++;
        
        /* Sécurité */
        if (iterations > dim * dim * nbcl * 10) {
            printf("ATTENTION: Trop d'itérations, arrêt\n");
            break;
        }
    }
    
    return iterations;
}








