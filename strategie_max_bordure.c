#include <stdlib.h>
#include "strategie_max_bordure.h"
#include "Liste_case.h"
#include "S_Zsg.h"

int exo_max_bordure(int **M, Grille *G, int dim, int nbcl, int aff)
{
    S_Zsg Z;
    init_Zsg(&Z, dim, nbcl);      // init zone

    int cdep = M[0][0];            // start color
    agrandit_Zsg(M, &Z, cdep, 0, 0);  // grow initial zone

    int fini = 0;
    int coups = 0;                  // move count

    while(!fini){
        int nb_cases[nbcl];
        for(int c = 0; c < nbcl; c++) nb_cases[c] = 0;

        // count border cells per color
        for(int c = 0; c < nbcl; c++){
            Elnt_liste *p = Z.B[c];
            while(p){
                nb_cases[c]++;
                p = p->suiv;
            }
        }

        // pick color with most border cells (not current)
        int cl_max = -1;
        int max = -1;
        for(int c = 0; c < nbcl; c++){
            if(nb_cases[c] > max && c != cdep){
                max = nb_cases[c];
                cl_max = c;
            }
        }

        if(cl_max == -1) break; // done

        // recolor current zone
        Elnt_liste *p = Z.Lzsg;
        while(p){
            M[p->i][p->j] = cl_max;
            p = p->suiv;
        }

        // expand new zone
        int k, l;
        while(!test_liste_vide(&Z.B[cl_max])){
            enleve_en_tete(&Z.B[cl_max], &k, &l);
            agrandit_Zsg(M, &Z, cl_max, k, l);
        }

        cdep = cl_max;  // update color
        coups++;         

        // check if any borders left
        fini = 1;
        for(int c = 0; c < nbcl; c++){
            if(!test_liste_vide(&Z.B[c])){
                fini = 0;
                break;
            }
        }

        // display grid 
        if(aff == 1 && G){
            for(int i = 0; i < dim; i++)
                for(int j = 0; j < dim; j++)
                    Grille_attribue_couleur_case(G, i, j, M[i][j]);
            Grille_redessine_Grille(G);
            Grille_attente_touche();
        }
    }

    return coups; 
}

