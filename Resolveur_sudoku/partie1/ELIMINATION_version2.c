/**
* @file sudoku.c
*
* @brief programme jeu de sudoku
*
* @author Evan Larose
*
* Ce programme propose de jouer une partie de sudoku dans le
* terminal en appelant une des 10 grilles disponibles
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/*definition des constantes*/
/**
 * 
 * @def N
 * 
 * @brief constante pour la hauteur et la largeur d'un bloc
 * 
*/
#define N 3
/**
 * 
 * @def TAILLE
 * 
 * @brief constante pour la taille d'un bloc
 * 
*/
#define TAILLE (N*N)

typedef int tTempGrille[TAILLE][TAILLE];

typedef struct{
    int valeur;
    bool candidats[TAILLE + 1];
    int nbCandidats;
}tCase2;

typedef tCase2 tGrille[TAILLE][TAILLE];

/*variables globales*/
float nbCandidatsElim = 0.;

/*definition des prototypes*/
int chargerGrille(tGrille g, char nomFichier[30]);
void afficherGrille(tGrille g);
bool possible(tGrille g, int ligne, int colonne, int valeur);
int initCandidats(tGrille g);
void ajouterCandidat(tCase2 *LaCase, int val);
void retirerCandidat(tCase2 *LaCase, int val);
bool estCandidat(tCase2 laCase, int val);
int nbCandidats(tCase2 laCase);
void affecterCandidat(tCase2 *laCase);
void retirerCandidatsLigColBloc(tGrille g, int lig, int col, int val);
void afficherStats(char nomFichier[30], int videsDepart, int nbCasesVides, int totalCandidats);

int main()
{
    tGrille g;
    bool progression;
    int nbCasesVides, videsDepart, totalCandidats;
    char nomFichier[30];

    printf("Nom du fichier ? ");
    scanf("%s", nomFichier);

    nbCasesVides = chargerGrille(g, nomFichier);
    videsDepart = nbCasesVides;
    totalCandidats = initCandidats(g);
    progression = true;

    afficherGrille(g);
    printf("\nGRILLE DE DEPART\n");

    while (nbCasesVides != 0 && progression)
    {
        progression = false;

        // technique du singleton nu
        for(int l = 0; l < TAILLE; l++)
        {
            for (int c = 0; c < TAILLE; c++)
            {
                if(g[l][c].valeur == 0)
                {
                    if (nbCandidats(g[l][c]) == 1)
                    {
                        affecterCandidat(&g[l][c]);
                        nbCasesVides = nbCasesVides - 1;
                        retirerCandidatsLigColBloc(g, l, c, g[l][c].valeur);
                        progression = true;
                    }
                }
            }
        }
    }
    afficherGrille(g);
    afficherStats(nomFichier, videsDepart, nbCasesVides, totalCandidats);
    
    return EXIT_SUCCESS;
}

/**
 * 
 * @fn int chargerGrille(tGrille g)
 * 
 * @brief recuperation de la grille appeler
 * 
 * @param g : grille a recuperer
 * @param nomFichier : nom du fichier a recuperer
 * 
 * @return retourne le nombre de case a remplir
 * 
 * recuperation de la grille à partir d'un fichier dont le nom est passé en parametre
 * 
 */
int chargerGrille(tGrille g, char nomFichier[30])
{
    int casesVides = 0;
    tTempGrille tab;
    FILE *f;
    f = fopen(nomFichier, "rb");
    if (f == NULL)
    {
        printf("\n ERREUR sur le fichier %s\n", nomFichier);
        exit(EXIT_FAILURE);
    }
    else
    {
        fread(tab, sizeof(int), TAILLE * TAILLE, f);
        fclose(f);
    }
    for (int l=0 ; l<TAILLE ; l++){
        for (int c=0 ; c<TAILLE ; c++){
            g[l][c].valeur = tab[l][c];
            
            /* si valeur de la case g[l][c] est egale a zero alors ajoute +1 a caseVides*/
            if (g[l][c].valeur == 0)
            {
                casesVides++ ;
            }
        }
    }
    return casesVides;
}

void afficherGrille(tGrille g)
{
    /*affichage des numéros de colonne de la grille et d'une barre horizontale*/
    printf("     1  2  3   4  5  6   7  8  9\n");
    printf("   +---------+---------+---------+");
    
    /*parcours des lignes du tableau g*/
    for (int l = 0; l<TAILLE; l++)
    {
        /*affichage des numéros de lignes de la grille*/
        printf("\n%d  |", l+1);
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
                printf(" %d ", g[l][c].valeur);
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
            printf("\n   +---------+---------+---------+");
        }
    }
}

/**
 * 
 * @fn void initCandidats(tCase2 *laCase)
 * 
 * @brief initialise les valeurs des candidats a false
 * 
 * @param laCase : structure d'une case qui contient sa valeur (ou 0 si c’est une case libre), un tableau de booléens tels que  t[i]=vrai  si la valeur  i  est candidate pour cette case (et  t[i]=faux sinon), et le nombre de candidats.
 * 
 * initialisation des candidats de la case passee en parametre a false
 * 
 */
int initCandidats(tGrille g)
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
    return totalCandidats;
}

/**
 * 
 * @fn void ajouterCandidat(tCase2 *laCase, int val)
 * 
 * @brief ajout d'un candidat
 * 
 * @param laCase : structure d'une case qui contient sa valeur (ou 0 si c’est une case libre), un tableau de booléens tels que  t[i]=vrai  si la valeur  i  est candidate pour cette case (et  t[i]=faux sinon), et le nombre de candidats.
 * @param val : valeur ajouter comme candidat
 * 
 * ajoute la valeur passee en parametre a la liste des candidats
 * 
 */
void ajouterCandidat(tCase2 *laCase, int val)
{
    laCase->candidats[val] = true;
    laCase->nbCandidats++ ;
}

/**
 * 
 * @fn void retirerCandidat(tCase2 *laCase, int val)
 * 
 * @brief retire un candidat
 * 
 * @param laCase : structure d'une case qui contient sa valeur (ou 0 si c’est une case libre), un tableau de booléens tels que  t[i]=vrai  si la valeur  i  est candidate pour cette case (et  t[i]=faux sinon), et le nombre de candidats.
 * @param val : valeur a supprimer des candidats
 * 
 * retire la valeur passee en parametre de la liste des candidats
 * 
 */
void retirerCandidat(tCase2 *laCase, int val)
{
    laCase->candidats[val] = false;
    laCase->nbCandidats-- ;
    nbCandidatsElim += 1;
}

/**
 * 
 * @fn bool estCandidat(tCase2 laCase, int val)
 * 
 * @brief verifie si val est candidat
 * 
 * @param laCase : structure d'une case qui contient sa valeur (ou 0 si c’est une case libre), un tableau de booléens tels que  t[i]=vrai  si la valeur  i  est candidate pour cette case (et  t[i]=faux sinon), et le nombre de candidats.
 * @param val : valeur dont il faut verifier si elle est candidat ou non
 * 
 * @return true si val est candidat, false sinon.
 * 
 * verifie si la valeur passee en parametre est un candidat de la case passee en parametre
 * 
 */
bool estCandidat(tCase2 laCase, int val)
{
    return laCase.candidats[val];
}

/**
 * 
 * @fn int nbCandidats(tCase2 laCase)
 * 
 * @brief recuperation du nombre de candidats
 * 
 * @param laCase : structure d'une case qui contient sa valeur (ou 0 si c’est une case libre), un tableau de booléens tels que  t[i]=vrai  si la valeur  i  est candidate pour cette case (et  t[i]=faux sinon), et le nombre de candidats.
 * 
 * @return retourne le nombre de candidats d'une case
 * 
 * recuperation du nombre de candidats d'une case passee en parametre
 * 
 */
int nbCandidats(tCase2 laCase)
{
    return laCase.nbCandidats;
}

void affecterCandidat(tCase2 *laCase)
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

/**
 * 
 * @fn bool possible(tGrille g, int ligne, int col, int val)
 * 
 * @brief fonction qui indique si il est possible d'insérer une valeur
 * 
 * @param g : grille a verifier
 * @param ligne : ligne a verifier
 * @param colonne : colonne a verifier
 * @param val : valeur a inserer
 * 
 * @return true si possible d'inserer la valeur false sinon
 * 
 * fonction qui indique si il est possible d'insérer une valeur a la case indiquée
 * en verifiant la presence de la valeur sur la ligne, la colonne et le bloc de cette case
*/
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

void afficherStats(char nomFichier[30], int videsDepart, int nbCasesVides, int totalCandidats)
{
    float casesRemplies = 0.;
    float tauxRemplissage = 0.;
    float pourcentageElim = 0.;

    casesRemplies = videsDepart - nbCasesVides;
    tauxRemplissage = casesRemplies*100/videsDepart;
    pourcentageElim = nbCandidatsElim*100/totalCandidats;

    printf("\n******\tRESULTATS POUR %s\t******\n", nomFichier);
    printf("\n Nombre de cases remplies = %0.f sur %d", casesRemplies, videsDepart);
    printf("\t Taux de remplissage = %0.3f %%\n", tauxRemplissage);
    printf("\n Nombre de candidats elimines = %0.f", nbCandidatsElim);
    printf("\t Pourcentage d'elimination = %0.3f %%\n", pourcentageElim);
}