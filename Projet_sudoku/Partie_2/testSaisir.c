#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define N 3

void saisir(int *adr_val);

int main()
{
    int numLigne, numColonne, valeur;
    printf("\nIndices de la ligne ? ");
    saisir(&numLigne);
    printf("\nIndices de la colonne ? ");
    saisir(&numColonne);
    printf("\nValeur a inserer ? ");
    saisir(&valeur);
}

void saisir(int *adr_val)
{
    char ch[50];
    scanf("%s", ch);
    while (*adr_val<1 || *adr_val>N*N || sscanf(ch, "%d", &*adr_val) == false)
    {
        do
        {
            printf("Ressaisir une valeur comprise entre 1 et 9 : ");
            scanf("%s", ch);
        } while (sscanf(ch, "%d", &*adr_val) == false); 
    }
}
