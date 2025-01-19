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
 * @def TAILLE
 * 
 * @brief constante pour la taille d'un bloc
 * 
*/
#define TAILLE 9
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
 * @typedef tGrille
 * 
 * @brief type tableau a deux dimensions de taille TAILLE*TAILLE
 * 
 * Le type tGrille sert de de stockage pour la grille de sudoku
*/
typedef int tGrille[TAILLE][TAILLE];

/*definition des prototypes*/
void chargerGrille(tGrille g);
void afficherGrille(tGrille g);
bool zeroDansGrille(tGrille g);
void saisir(int *adr_valeur);
bool possible(tGrille g, int ligne, int colonne, int valeur);

/**
 * 
 * @fn int main()
 * 
 * @brief Programme principal.
 * 
 * @return Code de sortie du programme (0 : sortie normale).
 * 
 * Le programme principal charge la grille puis l'affiche.
 * Il demande ensuite la saisie de la ligne et de la colonne et teste si la case est vide ou non.
 * Si la case est vide il demande la saisie de la valeur a insérer, puis teste la possibilité de le placé dans la case.
 * Puis il repete les actions de l'affichage a la verification de la possibilite d'insertion tant qu'il y a un zero dans la grille.
 * Enfin quand il n'y a plus de zero dans la grille il affiche la grille et un message de fin de partie.
 * 
*/ 
int main()
{
    /*definition des variables*/
    tGrille grille1;
    int numLigne, numColonne, valeur;

    /*appel de la procdure chargerGrille*/
    chargerGrille(grille1);

    while(zeroDansGrille(grille1))
    {
        /*appel de la procdure afficherGrille*/
        afficherGrille(grille1);
        
        /*saisie des indices de case et appel de la procedure saisir*/
        printf("\nIndice de la ligne ? ");
        saisir(&numLigne);
        printf("\nIndice de la colonne ? ");
        saisir(&numColonne);

        /*verification de la possiblité d'inserer une valeur dans la case choisie*/
        if(grille1[numLigne-1][numColonne-1] != 0)
        {
            /*message indiquant que la case choisie est deja occupée*/
            printf("\nIMPOSSIBLE, la case n'est pas libre.\n");
        }
        else
        {
            /*saisie de la valeur à inserer dans la case choisie et appel de la procedure saisir*/
            printf("\nValeur a inserer ? ");
            saisir(&valeur);

            /*verification de la possibilité d'inserer la valeur choisie dans la case*/
            if (possible(grille1, numLigne-1, numColonne-1, valeur) == true)
            {
                /*insertion de la valeur choisie dans la grille*/
                grille1[numLigne-1][numColonne-1] = valeur;
            }
        }
    }
    /*affichage final de la grille*/
    afficherGrille(grille1);
    /*message de fin de partie*/
    printf("\nFELICITATION !!!\nGrille pleine, fin de partie\n");
    return EXIT_SUCCESS;
}


/**
 * 
 * @fn void chargerGrille(tGrille g)
 * 
 * @brief recuperation de la grille appeler.
 * 
 * @param g : grille a recuperer.
 * 
 * recuperation de la grille à partir d'un fichier dont le nom est lu au clavier.
 * 
 */
void chargerGrille(tGrille g)
{
    char nomFichier[30];
    FILE *f;
    tGrille tab;
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
        fclose(f);
    }
    for (int l = 0; l < TAILLE; l++)
    {
        for (int c = 0; c < TAILLE; c++)
        {
            g[l][c] = tab[l][c];
        }
        
    }
    
}

/**
 * 
 * @fn void afficherGrille(tGrille g)
 * 
 * @brief affiche la grille.
 * 
 * @param g : grille a afficher.
 * 
 * procedure qui affiche la grille passée en paramètre.
 * 
*/
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
            if (g[l][c] == 0)
            {
                printf(" . ");
            }
            /*affiche la valeur de g[l][c]*/
            else
            {
                printf(" %d ", g[l][c]);
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
 * @fn bool zeroDansGrille(tGrille g)
 * 
 * @brief Fonction qui indique si il reste un zero dans la grille.
 * 
 * @param g : grille a verifier.
 * 
 * @return true si il reste un zero, false sinon.
 * 
 * fonction qui verifie si la grille est remplie ou non en verifiant la présence d'un zero
 */
bool zeroDansGrille(tGrille g)
{
    /* parcours du tableau */
    for (int l=0 ; l<TAILLE ; l++){
        for (int c=0 ; c<TAILLE ; c++){
            /* si valeur de la case g[l][c] est egale a zero la fonction s'arrete en retournant true */
            if (g[l][c] == 0)
            {
                return true;
            }
        }
    }
    /*pas de zero dans la grille donc retourne false*/
    return false;
}

/**
 * 
 * @fn void saisir(int *adr_val)
 * 
 * @brief permet de saisir un valeur entre 1 et 9
 * 
 * @param adr_val : valeur saisie au clavier
 * 
 * procedure permettant de saisir les valeurs de colonne, ligne et valeur à insérer
*/
void saisir(int *adr_val)
{
    char ch[50];
    scanf("%s", ch);
    /* les strcmp() verifie que la valeur saisie au clavier est différente de + et - car sinon + et - sont pris en compte comme un 1 || sscanf vérifie si la chaine saisie est un nombre || les deux dernieres condiititons vérifient que la valeur saisie est comprise entre 1 et 9*/
    while(strcmp(ch, "+") == 0 || strcmp(ch, "-") == 0 || sscanf(ch, "%d", adr_val) == false || *adr_val<1 || *adr_val>N*N)
    {
        printf("Ressaisir un nombre compris entre 1 et 9 : ");
        scanf("%s", ch);
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
        if(g[l][col] == val)
        {
            printf("\nERREUR valeur deja presente sur la colonne de la case\n");
            return false;
        }
    }
    /*verification de la ligne de la case(c parcours la colonne)*/
    for (int c = 0; c < TAILLE; c++)
    {
        if (g[ligne][c] == val)
        {
            printf("\nERREUR valeur deja presente sur la ligne de la case\n");
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
                printf("\nERREUR valeur deja presente dans le bloc de la case\n");
                return false;                
            }
        }   
    }
    return true;
}