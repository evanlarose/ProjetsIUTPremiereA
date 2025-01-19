#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 4
#define TAILLE (N*N)

typedef int tGrille[TAILLE][TAILLE];

void chargerGrille(tGrille g);
void afficherGrille(tGrille g);
bool backtracking(tGrille grille, int numeroCase);
bool possible(tGrille g, int ligne, int colonne, int valeur);

int main()
{
    tGrille grille;
    //int numLigne, numColonne, valeur;

    chargerGrille(grille);
    afficherGrille(grille);
    for(int i = 0; i<TAILLE; i++)
    {
        backtracking(grille, i);
    }
    afficherGrille(grille);

    return EXIT_SUCCESS;
}

void chargerGrille(tGrille g)
{
    char nomFichier[30];
    FILE *f;
    printf("Nom du fichier ? ");
    scanf("%s", nomFichier);
    f = fopen(nomFichier, "rb");
    if (f == NULL)
    {
        printf("\n ERREUR sur le fichier %s\n", nomFichier);
        exit(EXIT_FAILURE);
    }
    else
    {
        fread(g, sizeof(int), TAILLE * TAILLE, f);
        fclose(f);
    }
}

void afficherGrille(tGrille g)
{
    /*affichage des numéros de colonne de la grille et d'une barre horizontale*/
    printf("     1  2  3  4   5  6  7  8   9  10 11 12  13 14 15 16\n");
    printf("   +------------+------------+------------+------------+");
    
    /*parcours des lignes du tableau g*/
    for (int l = 0; l<TAILLE; l++)
    {
        /*affichage des numéros de lignes de la grille*/
        if(l >= 9) {
            printf("\n%d |", l+1);
        }
        else{
            printf("\n%d  |", l+1);
        }
        /*parcours des colonnes du tableau g*/
        for (int c = 0; c<TAILLE; c++)
        {
            /*remplace les zero par des points a l'affichage*/
            if (g[l][c] == 0)
            {
                printf(" . ");
            }
            /*affiche la valeur de g[l][c]*/
            else
            {
                if (g[l][c] > 9)
                {
                   printf(" %d", g[l][c]); 
                }
                else
                {
                    printf(" %d ", g[l][c]);
                }
            }
            /*affichage du caractère | toutes les trois colonnes*/
            if ((c+1)%N == 0)
            {
                printf("|");
            }
        }
        /*affichage d'une barre horizontal toutes les 3 colonnes*/
        if ((l+1)%N == 0)
        {
            printf("\n   +------------+------------+------------+------------+");
        }
    }
    printf("\n");
}

bool backtracking(tGrille grille, int numeroCase)
{
    int ligne;
    int colonne;
    bool resultat;

    resultat = false;
    if(numeroCase == TAILLE * TAILLE)
    {
        //on a traité toutes les cases, la grille est résolue
        resultat = true;
    }
    else
    {
        // On récupère les "coordonnées" de la case
        ligne = numeroCase / TAILLE;
        colonne = numeroCase % TAILLE;
        if (grille[ligne][colonne] != 0)
        {
            // Si la case n’est pas vide, on passe à la suivante
            // (appel récursif)
            return backtracking(grille, numeroCase+1);
        }
        else
        {
            for(int valeur = 0; valeur<TAILLE; valeur++)
            {
                if(possible(grille, ligne, colonne, valeur))
                {
                    // Si la valeur est autorisée on  l'inscrit 
                    //dans la case...
                    grille[ligne][colonne] = valeur;
                    // ... et on passe à la suivante : appel récursif
                    // pour voir si ce choix est bon par la suite
                    if(backtracking(grille, numeroCase+1))
                    {
                        return true;
                    }
                    else
                    {
                        grille[ligne][colonne] = 0;
                    }
                }
            }
        }
    }
    return resultat;
}

bool possible(tGrille g, int ligne, int col, int val)
{
    /*verification de la colonne de la case(l parcours la ligne)*/
    for(int l = 0; l<TAILLE; l++)
    {
        if(g[l][col] == val)
        {
            return false;
        }
    }
    /*verification de la ligne de la case(c parcours la colonne)*/
    for (int c = 0; c < TAILLE; c++)
    {
        if (g[ligne][c] == val)
        {
            return false;
        }
    }
    /*verification du bloc de la case(l parcours des lignes du bloc et c des colonnes du bloc)*/
    for (int l = ligne-(ligne%N); l < (ligne-(ligne%N))+N; l++)
    {
        for (int c = col-(col%N); c < (col-(col%N))+N; c++)
        {
            if (g[l][c] == val)
            {
                return false;                
            }
        }   
    }
    return true;
}
