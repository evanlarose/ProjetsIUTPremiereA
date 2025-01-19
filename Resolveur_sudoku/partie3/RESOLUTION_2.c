/**
 * @brief amelioration de la resolution par l'ajout des techniques singleton nu et singleton cache
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 4
#define TAILLE (N*N)

typedef struct{
    int valeur;
    bool candidats[TAILLE + 1];
    int nbCandidats;
}tCase2;

typedef tCase2 tGrille[TAILLE][TAILLE];
typedef int tTempGrille[TAILLE][TAILLE];

int chargerGrille(tGrille g);
void initCandidats(tGrille g);
void afficherGrille(tGrille g);
int nbCandidats(tCase2 laCase);
void affecterSingletonNu(tCase2 *laCase);
void ajouterCandidat(tCase2 *LaCase, int val);
void retirerCandidat(tCase2 *LaCase, int val);
bool estCandidat(tCase2 laCase, int val);
void retirerCandidatsLigColBloc(tGrille g, int lig, int col, int val);
bool backtracking(tGrille grille, int numeroCase);
bool possible(tGrille g, int ligne, int colonne, int valeur);
bool possibleSingletonLig(tGrille g, int ligne, int col, int val);
bool possibleSingletonCol(tGrille g, int lig, int col, int val);
bool possibleSingletonBloc(tGrille g, int ligne, int col, int val);
void affecterSingletonCache(tCase2 *laCase, int val);

int main()
{
    tGrille grille;
    bool progression;
    int nbCasesVides = 0;

    progression = true;

    nbCasesVides = chargerGrille(grille);
    initCandidats(grille);
    printf("\nGrille de depart\n");
    afficherGrille(grille);

    clock_t begin = clock();
    while (progression && nbCasesVides != 0)
    {
        progression = false;

        // technique du singleton nu
        for(int l = 0; l < TAILLE; l++)
        {
            for (int c = 0; c < TAILLE; c++)
            {
                if(grille[l][c].valeur == 0)
                {
                    if (nbCandidats(grille[l][c]) == 1)
                    {
                        affecterSingletonNu(&grille[l][c]);
                        nbCasesVides = nbCasesVides - 1;
                        retirerCandidatsLigColBloc(grille, l, c, grille[l][c].valeur);
                        progression = true;
                    }
                }
            }
        }
        progression = false;
            
        //technique du singleton caché
        for(int l = 0; l < TAILLE; l++)
        {
            for (int c = 0; c < TAILLE; c++)
            {
                if(grille[l][c].valeur == 0)
                {
                    for(int i = 1; i<TAILLE+1; i++)
                    {
                        if(estCandidat(grille[l][c], i) && (possibleSingletonLig(grille, l, c, i) || possibleSingletonCol(grille, l, c, i) || possibleSingletonBloc(grille, l, c, i)))
                        {
                            affecterSingletonCache(&grille[l][c], i);
                            nbCasesVides = nbCasesVides - 1;
                            retirerCandidatsLigColBloc(grille, l, c, grille[l][c].valeur);
                            progression = true;
                        }
                    }
                }
            }
        }
    }

    if(nbCasesVides != 0)
    {
        for(int i = 0; i<TAILLE; i++)
        {
            backtracking(grille, i);
        }
    }
    clock_t end = clock();
    
    printf("Grille resolu\n\n");
    afficherGrille(grille);
    double  tmpsCPU = (end - begin)*1.0 / CLOCKS_PER_SEC;
    printf( "Resolution de la grille en %.6f sec\n", tmpsCPU);

    return EXIT_SUCCESS;
}

int chargerGrille(tGrille g)
{
    int vides = 0;
    char nomFichier[30];
    tTempGrille tab;
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
        fread(tab, sizeof(int), TAILLE * TAILLE, f);
    }
    fclose(f);
    for (int l=0 ; l<TAILLE ; l++){
        for (int c=0 ; c<TAILLE ; c++){
            g[l][c].valeur = tab[l][c];

            if (g[l][c].valeur == 0)
            {
                vides++ ;
            }
        }
    }
    return vides;
}

void initCandidats(tGrille g)
{
    int totalCandidats = 0;
    for (int l = 0; l < TAILLE; l++)
    {
        for (int c = 0; c < TAILLE; c++)
        {
            g[l][c].nbCandidats = 0;
            if (g[l][c].valeur == 0)
            {
                for(int i = 1; i<TAILLE+1; i++)
                {
                    if (possible(g, l, c, i))
                    {
                        ajouterCandidat(&g[l][c], i);
                        totalCandidats++ ;
                    }
                    else
                    {
                        g[l][c].candidats[i] = false;
                    }
                }
            }
        }
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
            if (g[l][c].valeur == 0)
            {
                printf(" . ");
            }
            /*affiche la valeur de g[l][c]*/
            else
            {
                if (g[l][c].valeur > 9)
                {
                   printf(" %d", g[l][c].valeur); 
                }
                else
                {
                    printf(" %d ", g[l][c].valeur);
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

int nbCandidats(tCase2 laCase)
{
    return laCase.nbCandidats;
}

void affecterSingletonNu(tCase2 *laCase)
{
    for(int i = 1; i<TAILLE+1; i++)
    {
        if(estCandidat(*laCase, i))
        {
            laCase->valeur = i;
            retirerCandidat(laCase, i);
        }
    }
}

void affecterSingletonCache(tCase2 *laCase, int val)
{
    laCase->valeur = val;
    retirerCandidat(laCase, val);
}

void ajouterCandidat(tCase2 *laCase, int val)
{
    laCase->candidats[val] = true;
    laCase->nbCandidats++ ;
}

void retirerCandidat(tCase2 *laCase, int val)
{
    laCase->candidats[val] = false;
    laCase->nbCandidats-- ;
}

void retirerCandidatsLigColBloc(tGrille g, int lig, int col, int val)
{
    /*retire le candidat de la colonne de la case(l parcours la ligne)*/
    for(int l = 0; l<TAILLE; l++)
    {
        if(g[l][col].valeur == 0 && estCandidat(g[l][col], val))
        {
            retirerCandidat(&g[l][col], val);
        }
    }
    /*retire le candidat de la ligne de la case(c parcours la colonne)*/
    for (int c = 0; c < TAILLE; c++)
    {
        if (g[lig][c].valeur == 0 && estCandidat(g[lig][c], val))
        {
            retirerCandidat(&g[lig][c], val);
        }
    }
    /*retire le candidat du bloc de la case(l parcours des lignes du bloc et c des colonnes du bloc)*/
    for (int l = lig-(lig%N); l < (lig-(lig%N))+N; l++)
    {
        for (int c = col-(col%N); c < (col-(col%N))+N; c++)
        {
            if (g[l][c].valeur == 0 && estCandidat(g[l][c], val))
            {
                retirerCandidat(&g[l][c], val);
            }
        }   
    }
}

bool estCandidat(tCase2 laCase, int val)
{
    return laCase.candidats[val];
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
        if (grille[ligne][colonne].valeur != 0)
        {
            // Si la case n’est pas vide, on passe à la suivante
            // (appel récursif)
            return backtracking(grille, numeroCase+1);
        }
        else
        {
            for(int valeur = 1; valeur<=TAILLE; valeur++)
            {
                if(possible(grille, ligne, colonne, valeur))
                {
                    // Si la valeur est autorisée on  l'inscrit 
                    //dans la case...
                    grille[ligne][colonne].valeur = valeur;
                    // ... et on passe à la suivante : appel récursif
                    // pour voir si ce choix est bon par la suite
                    if(backtracking(grille, numeroCase+1))
                    {
                        return true;
                    }
                    else
                    {
                        grille[ligne][colonne].valeur = 0;
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
        if(g[l][col].valeur == val)
        {
            return false;
        }
    }
    /*verification de la ligne de la case(c parcours la colonne)*/
    for (int c = 0; c < TAILLE; c++)
    {
        if (g[ligne][c].valeur == val)
        {
            return false;
        }
    }
    /*verification du bloc de la case(l parcours des lignes du bloc et c des colonnes du bloc)*/
    for (int l = ligne-(ligne%N); l < (ligne-(ligne%N))+N; l++)
    {
        for (int c = col-(col%N); c < (col-(col%N))+N; c++)
        {
            if (g[l][c].valeur == val)
            {
                return false;                
            }
        }   
    }
    return true;
}

bool possibleSingletonLig(tGrille g, int ligne, int col, int val)
{
    /*verification de la ligne de la case(c parcours la colonne)*/
    for (int c = 0; c<TAILLE; c++)
    {
        if(c != col){
            if (g[ligne][c].candidats[val])
            {
                return false;
            }
        }
    }
    return true;
}

bool possibleSingletonCol(tGrille g, int lig, int col, int val)
{
    /*verification de la colonne de la case(l parcours la ligne)*/
    for(int l = 0; l<TAILLE; l++)
    {
        if(l != lig){
            if(g[l][col].candidats[val])
            {
                return false;
            }
        }
    }
    return true;
}

bool possibleSingletonBloc(tGrille g, int ligne, int col, int val)
{
    /*verification du bloc de la case(l parcours des lignes du bloc et c des colonnes du bloc)*/
    for (int l = ligne-(ligne%N); l < (ligne-(ligne%N))+N; l++)
    {
        for (int c = col-(col%N); c < (col-(col%N))+N; c++)
        {
            if (g[l][c].candidats[val])
            {
                 return false;                
            } 
        }
    }
    return true;
}