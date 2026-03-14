#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "API_Grille.h"

/* Directions */
static int dx[4] = {0, -1, 0, 1};
static int dy[4] = {1, 0, -1, 0};

static bool valid(int n, int x, int y) {
    return x >= 0 && x < n && y >= 0 && y < n;
}

static int flood(int n, int **M, int x, int y, int oldColor, int newColor) {
    if (!valid(n, x, y) || M[x][y] != oldColor)
        return 0;
    M[x][y] = newColor;
    int added = 1;
    for (int i = 0; i < 4; i++)
        added += flood(n, M, x + dx[i], y + dy[i], oldColor, newColor);
    return added;
}

static void copyMatrix(int n, int **src, int **dest) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            dest[i][j] = src[i][j];
}

static bool solved(int n, int **M) {
    int c = M[0][0];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (M[i][j] != c)
                return false;
    return true;
}

/* Compte le nombre de régions connectées de la même couleur */
static int countRegions(int n, int **M) {
    int **vis = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
        vis[i] = calloc(n, sizeof(int));

    int regions = 0;
    int *sx = malloc(n * n * sizeof(int));
    int *sy = malloc(n * n * sizeof(int));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (!vis[i][j]) {
                int color = M[i][j];
                int top = 0;
                sx[top] = i;
                sy[top] = j;
                top++;
                vis[i][j] = 1;

                while (top) {
                    top--;
                    int x = sx[top], y = sy[top];
                    for (int k = 0; k < 4; k++) {
                        int nx = x + dx[k], ny = y + dy[k];
                        if (valid(n, nx, ny) && !vis[nx][ny] && M[nx][ny] == color) {
                            vis[nx][ny] = 1;
                            sx[top] = nx;
                            sy[top] = ny;
                            top++;
                        }
                    }
                }
                regions++;
            }
        }
    }

    for (int i = 0; i < n; i++) free(vis[i]);
    free(vis);
    free(sx);
    free(sy);
    return regions;
}
/*
 * Fonction implémentant une autre stratégie de résolution de la grille.
 * Une description détaillée de la stratégie et de son fonctionnement
 * est présentée dans le rapport.
 */

int sequence_greedy_astar(int **M, int n, int nbcl, Grille *G, int aff) {
    int moves = 0;

    int **temp = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
        temp[i] = malloc(n * sizeof(int));

    while (!solved(n, M)) {
        int current = M[0][0];
        int bestColor = -1;
        int maxAdded = -1;
        int bestRegions = 1000000000;

        /*  Déterminer les couleurs adjacentes à la Zsg */
        bool adjacentColors[nbcl];
        for (int c = 0; c < nbcl; c++) adjacentColors[c] = false;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (M[i][j] == current) {
                    for (int k = 0; k < 4; k++) {
                        int ni = i + dx[k], nj = j + dy[k];
                        if (valid(n, ni, nj) && M[ni][nj] != current)
                            adjacentColors[M[ni][nj]] = true;
                    }
                }
            }
        }

        /*  Tester uniquement les couleurs adjacentes */
        for (int c = 0; c < nbcl; c++) {
            if (!adjacentColors[c]) continue;

            copyMatrix(n, M, temp);
            int added = flood(n, temp, 0, 0, current, c);
            int regions = countRegions(n, temp);

            if (added > maxAdded || (added == maxAdded && regions < bestRegions)) {
                maxAdded = added;
                bestRegions = regions;
                bestColor = c;
            }
        }

        /*  Sécurité : dernier coup si rien n’est trouvé */
        if (bestColor == -1) {
            for (int i = 0; i < n; i++) {
                int found = 0;
                for (int j = 0; j < n; j++) {
                    if (M[i][j] != current) {
                        bestColor = M[i][j];
                        found = 1;
                        break;
                    }
                }
                if (found) break;
            }
        }

        /*  Appliquer le meilleur coup */
        flood(n, M, 0, 0, current, bestColor);
        moves++;

        /*  Affichage si demandé */
        if (aff && G) {
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    Grille_attribue_couleur_case(G, i, j, M[i][j]);
            Grille_redessine_Grille(G);
            Grille_attente_touche();
        }
    }

    /* Libération mémoire */
    for (int i = 0; i < n; i++) free(temp[i]);
    free(temp);

    return moves;
}

