#ifndef S_ZSG_H
#define S_ZSG_H

#include "Liste_case.h"

/* Structure conforme à l'énoncé */
typedef struct {
    int dim;        /* dimension de la grille */
    int nbcl;       /* nombre de couleurs */
    ListeCase Lzsg; /* Liste des cases de la zone Zsg */
    ListeCase *B;   /* Tableau de listes de cases de la bordure, taille nbcl */
    int **App;      /* Tableau d'appartenance : -2 non rencontré, -1 Zsg, cl bordure */
} S_Zsg;

/* Initialisation de la structure */
void init_Zsg(S_Zsg *Z, int dim, int nbcl);

/* Ajoute une case (i,j) dans Lzsg en O(1) */
int ajoute_Zsg(S_Zsg *Z, int i, int j);

/* Ajoute une case (i,j) dans la bordure B[cl] en O(1) */
int ajoute_Bordure(S_Zsg *Z, int i, int j, int cl);

/* Tests O(1) */
int appartient_Zsg(S_Zsg *Z, int i, int j);
int appartient_Bordure(S_Zsg *Z, int cl, int i, int j);

/* Agrandit la Zsg en partant de la case (k,l) de couleur cl.
   Retourne le nombre de cases ajoutées à Lzsg. */
int agrandit_Zsg(int **M, S_Zsg *Z, int cl, int k, int l);

/* Recolore toutes les cases de la Zsg dans M à la couleur cl (utile avant agrandissement). */
void recolor_Zsg(int **M, S_Zsg *Z, int cl);



#endif
