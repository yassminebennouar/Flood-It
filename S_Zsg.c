#include <stdlib.h>
#include <stdio.h>
#include "S_Zsg.h"
#include "Liste_case.h"

/* init_Zsg: initialize lists and the App array */
void init_Zsg(S_Zsg *Z, int dim, int nbcl){
    int i,j;
    Z->dim = dim;
    Z->nbcl = nbcl;

    init_liste(&(Z->Lzsg));

    Z->B = (ListeCase*) malloc(nbcl * sizeof(ListeCase));
    if (Z->B == NULL) {
        fprintf(stderr, "init_Zsg: malloc B failed\n");
        exit(1);
    }
    for(i=0;i<nbcl;i++) init_liste(&(Z->B[i]));

    Z->App = (int**) malloc(dim * sizeof(int*));
    if (Z->App == NULL) {
        fprintf(stderr, "init_Zsg: malloc App failed\n");
        exit(1);
    }
    for(i=0;i<dim;i++){
        Z->App[i] = (int*) malloc(dim * sizeof(int));
        if (Z->App[i] == NULL) {
            fprintf(stderr, "init_Zsg: malloc App[%d] failed\n", i);
            exit(1);
        }
        for(j=0;j<dim;j++) Z->App[i][j] = -2; /* -2 = not visited */
    }
}

/* ajoute_Zsg: add a cell at the head of Lzsg (O(1)) */
int ajoute_Zsg(S_Zsg *Z, int i, int j){
    int r = ajoute_en_tete(&(Z->Lzsg), i, j);
    Z->App[i][j] = -1; /* -1 = belongs to Zsg */
    return r;
}

/* ajoute_Bordure: add cell (i,j) to B[cl] if not already visited */
int ajoute_Bordure(S_Zsg *Z, int i, int j, int cl){
    if (Z->App[i][j] != -2) return 0; /* already visited */
    Z->App[i][j] = cl;
    return ajoute_en_tete(&(Z->B[cl]), i, j);
}

/* Membership tests in O(1) */
int appartient_Zsg(S_Zsg *Z, int i, int j){
    return (Z->App[i][j] == -1);
}
int appartient_Bordure(S_Zsg *Z, int cl, int i, int j){
    return (Z->App[i][j] == cl);
}

/* recolor_Zsg: recolor all cells of Lzsg with color cl */
void recolor_Zsg(int **M, S_Zsg *Z, int cl){
    Elnt_liste *cur = Z->Lzsg;
    while(cur != NULL){
        M[cur->i][cur->j] = cl;
        cur = cur->suiv;
    }
}

/*
 agrandit_Zsg:
 Explore the connected component of color cl starting from (k,l).
 Add matching cells to Lzsg and neighbors of different colors to borders.
 Return the number of cells added to Lzsg.
*/
int agrandit_Zsg(int **M, S_Zsg *Z, int cl, int k, int l){
    int dim = Z->dim;
    int added = 0;

    /* directions */
    int di[4] = {1,-1,0,0};
    int dj[4] = {0,0,1,-1};

    /* if already in Zsg, nothing to do */
    if (Z->App[k][l] == -1) return 0;

    /* temporary list for flood fill */
    ListeCase Ltmp;
    init_liste(&Ltmp);

    /* start exploration if not visited or already in border cl */
    if (Z->App[k][l] == -2 || Z->App[k][l] == cl) {
        ajoute_en_tete(&Ltmp, k, l);
    } else {
        return 0;
    }

    /* flood fill on color cl */
    while(!test_liste_vide(&Ltmp)){
        int i,j;
        enleve_en_tete(&Ltmp, &i, &j);

        /* skip if already in Zsg */
        if (Z->App[i][j] == -1) continue;

        if (M[i][j] == cl) {
            ajoute_Zsg(Z, i, j);
            added++;

            /* explore neighbors */
            for(int d=0; d<4; d++){
                int ni = i + di[d];
                int nj = j + dj[d];
                if (ni < 0 || nj < 0 || ni >= dim || nj >= dim) continue;

                if (Z->App[ni][nj] == -1) continue;

                if (M[ni][nj] == cl) {
                    if (Z->App[ni][nj] == -2) {
                        ajoute_en_tete(&Ltmp, ni, nj);
                    }
                } else {
                    int cl2 = M[ni][nj];
                    if (Z->App[ni][nj] == -2) {
                        ajoute_Bordure(Z, ni, nj, cl2);
                    }
                }
            }
        } else {
            /* cell is a border candidate */
            int cl2 = M[i][j];
            if (Z->App[i][j] == -2) {
                ajoute_Bordure(Z, i, j, cl2);
            }
        }
    }

    return added;
}


