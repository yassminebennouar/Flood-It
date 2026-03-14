#include <stdlib.h>
#include "sequence_alea_rapide.h"
#include "Liste_case.h"
#include "S_Zsg.h"

/* Wrapper: random sequence with optional display */
int sequence_alea_rapide(int **M, Grille *G, int dim, int nbcl, int aff){
    S_Zsg Z;
    init_Zsg(&Z, dim, nbcl);

    int coups=0, cdep=M[0][0];
    agrandit_Zsg(M,&Z,cdep,0,0);

    while(1){
        int cl; do{ cl=rand()%nbcl; }while(cl==cdep);

        Elnt_liste *p=Z.Lzsg;
        while(p){ M[p->i][p->j]=cl; p=p->suiv; }

        int k,l;
        while(!test_liste_vide(&Z.B[cl])){
            enleve_en_tete(&Z.B[cl],&k,&l);
            agrandit_Zsg(M,&Z,cl,k,l);
        }

        cdep=cl; coups++;

        int fini=1;
        for(int i=0;i<dim;i++){
            for(int j=0;j<dim;j++){
                if(Z.App[i][j]!=-1){ fini=0; break; }
            }
            if(!fini) break;
        }
        if(fini) break;

        if(aff && G){
            for(int i=0;i<dim;i++)
                for(int j=0;j<dim;j++)
                    Grille_attribue_couleur_case(G,i,j,M[i][j]);
            Grille_redessine_Grille(G);
            Grille_attente_touche();
        }
    }

    return coups;
}


