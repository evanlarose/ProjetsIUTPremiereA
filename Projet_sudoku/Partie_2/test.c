#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define TAILLE 9
#define N 3

//definition des types utilisateurs
typedef int tGrille[TAILLE][TAILLE];

bool zeroDansGrille(tGrille g);

int main()
{
    tGrille  tab ={
                    {1, 4, 3, 1, 1, 3, 1, 4, 3},      // premiere ligne
                    {5, 8, 7, 1, 4, 3, 1, 4, 3},      // deuxieme ligne
                    {1, 4, 4, 5, 8, 7, 1, 4, 9},    // etc.
                    {2, 9, 9, 5, 8, 7, 1, 4, 9},
                    {1, 2, 3, 5, 8, 7, 1, 4, 9},
                    {1, 4, 3, 5, 8, 7, 1, 4, 9},
                    {1, 4, 9, 5, 7, 7, 1, 4, 9},
                    {1, 4, 9, 5, 8, 7, 1, 4, 9},
                    {1, 4, 3, 5, 8, 4, 1, 4, 9}
                 };

    printf("%d", zeroDansGrille(tab));

    printf("     1  2  3   4  5  6   7  8  9\n");
    printf("   +---------+---------+---------+");
    for (int i = 0; i<TAILLE; i++)
    {
        printf("\n%d  |", i+1);
        for (int j = 0; j<TAILLE; j++)
        {
            if (tab[i][j] == 0)
            {
                printf(" . ");
            }
            else if ((j+1)%N == 0)
            {
                printf(" %d |", tab[i][j]);
            }
            else
            {
                printf(" %d ", tab[i][j]);
            }
        }
        if ((i+1)%N == 0)
        {
            printf("\n   +---------+---------+---------+");
        }
    }
}

bool zeroDansGrille(tGrille g)
{
    for (int i=0 ; i<TAILLE ; i++){
        for (int j=0 ; j<TAILLE ; j++){
            if (g[i][j] == 0)
            {
                return true;
            }
        }
    }
    return false;
}