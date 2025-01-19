/**
* \page Général 
* 
* \author MABIT Baptiste
* \version 1.0
* \date 1 décembre 2023
*
* Programme d'un sudoku en langage C à résoudre par l'utilisateur dans le terminal.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
* \def N 
* \brief Taille d'un carré de la grille
*/
#define N 3 // taille des carrés 

/**
* \def TAILLE
* \brief Taille des tableaux et valeur maximal des valeurs qu'on peut rentrer dans la grille.
*
* correspond au carré de N.
*/
#define TAILLE N*N // nombre de ligne et de colonne

/**
* \typedef tGrille
* \brief grille du sudoku.
*
* Composé d'un tableau de TAILLE éléments contenants eux mêmes TAILLE éléments.
*/

typedef struct{
    int valeur;
    int candidats[TAILLE];
    int nbCandidat;
}tCase1;

typedef tCase1 tGrille[TAILLE][TAILLE];

typedef int tGrilleTemp[TAILLE][TAILLE];

void chargerGrille(tGrilleTemp gT, tGrille g, char nomGrille[15]);
void afficherGrille(tGrille grille);
void saisir(int *valeur);
int possible(tGrille grille,int ligne, int colonne, int valeur);
int parcourCarre(int ligneDepart, int colonneDepart, int valeur,tGrille grille);
int grillePleine(tGrille grille);
void ajouterCandidat(tCase1 *laCase, int val);
void retirerCandidat(tCase1 *laCase, int val,int nbCandidatsElimines);
bool estCandidat(tCase1 laCase, int val);
int nbCandidat(tCase1 laCase);
void initialisationCandidats(tGrille g, float nbCandidatsTotal);
void rechercheCandidats(tGrille *g,float nbCasesRemplies, float nbCandidatsElimines,float nbCasesVides);
void candidatsLigne(tGrille *g,int ligne, float nbCandidatsElimines);
void candidatsColonne(tGrille *g,int colonne,float nbCandidatsElimines);
void candidatsCarre(tGrille *g, int ligne, int colonne,float nbCandidatsElimines);
void afficherStats(char nomGrille[15], float nbCasesVides, float nbCasesRemplies, float nbCandidatsTotal, float nbCandidatsElimines);

/**
* \fn int main()
* \brief Programme principal.
* \return Code de sortie du programme (0 : sortie normale).
* 
* Permet au joueur d'entrer des valeurs dans les cases
* du sudoku tant que celui-ci n'est pas remplie.
*/

int main(){
    tGrille grille1;
    tGrilleTemp grilleTemp1;
    float nbCandidatsTotal = 0;
    float nbCasesRemplies = 0;
    float nbCasesVides = 0;
    float nbCandidatsElimines = 0;
    char nomGrille[15];
    chargerGrille(grilleTemp1, grille1, nomGrille); // chargement de la grille
    initialisationCandidats(grille1,nbCandidatsTotal);
    afficherGrille(grille1);
    while(grillePleine(grille1) == 0){ //tant qua la grille n'est pas pleine
        rechercheCandidats(&grille1,nbCasesRemplies,nbCandidatsElimines,nbCasesVides);
    }
    afficherGrille(grille1);
    printf("Grille pleine, fin de partie");
    return EXIT_SUCCESS;
}

/**
* \fn void chargerGrille(tGrille g).
* \brief Charge la grille contenue dans le fichier .sud qu'on lui demande.
* \param g : paramètre de sortie qui représente la grille chargée.
*/
void chargerGrille(tGrilleTemp gT, tGrille g,char nomGrille[15]){
    // charge la grille présente dans le fichier qu'on appelle (donné par le prof)
    char nomFichier[30];
    FILE * f;

    for(int i = 0;i<TAILLE;i++){
        for(int j = 0;j<TAILLE;j++){
            g[i][j].valeur = 0;
        }
    }

    printf (" Nom du fichier ? ");
    scanf("%s",nomFichier);
    nomGrille = nomFichier,
    f = fopen(nomFichier, "rb");
    if (f == NULL){
        printf("\n ERREUR sur le fichier %s",nomFichier);
        exit(EXIT_FAILURE);
    }
    else{
        fread(gT,sizeof(int),TAILLE * TAILLE,f);        
        fclose(f);
    }
    for(int i = 0;i<TAILLE;i++){
        for(int j = 0;j<TAILLE;j++){
            g[i][j].valeur = gT[i][j];
        }
    }
    printf("grille chargee \n");
}

/**
* \fn void afficherGrille(tGrille grille)
* \brief Affiche la grille de manière graphique dans le terminal.
* \param grille : paramètre d'entrée qui représente la grille qu'on a choisie.
*
* Affiche toute la grille avec les indices des lignes des | pour délimiter les colonnes des carrés 
* et la ligneSeparation pour délimiter la ligne des carrés*
*/
void afficherGrille(tGrille grille){
    
    int l,c;
    char ligneDepart[50] = "     1  2  3   4  5  6   7  8  9  \n";
    char ligneSeparation[50] = "   +---------+---------+---------+ \n";

    printf("%s",ligneDepart);
    printf("%s",ligneSeparation);
    for (l = 0; l < TAILLE ; l++){
        printf("%d  ",l+1); // affiche le numéro de la ligne
        for(c=0;c<TAILLE; c++){
            if(c%N == 0){ 
                printf("|"); //toute les 3 valeurs affiche un |
            }
            if(grille[l][c].valeur == 0){
                printf(" . "); // affiche un . à la place des 0 (ne les remplaces pas dans la grille)
            }
            else{
            printf(" %d ",grille[l][c].valeur);
            }
        }
        printf("|"); // affichage de fin de ligne 
        printf("\n");
        if(l%N == 2){
            printf("%s",ligneSeparation);
        }
    }
}

/**
* \fn grillePleine(tGrille grille)
* \brief regarde si la grille est pleine.
* \param grille : paramètre d'entrée qui représente la grille du sudoku.
* \return 0 si la grille n'est pas pleine, 1 si la grille est pleine.
*/
int grillePleine(tGrille grille){
    /*Parcours toute la grille
    renvoie 0 si il trouve un 0
    renvoie 1 sinon*/
    int l,c;
    for(l=0;l<TAILLE;l++){
        for(c=0;c<TAILLE;c++){
            if(grille[l][c].valeur == 0){
                return 0;
            }
        }
    }
    return 1;
}

/*
* résolution
*/

void ajouterCandidat(tCase1 *laCase, int val){
    int i = 0;
    if(val > laCase->candidats[laCase->nbCandidat-1]){
        laCase->candidats[laCase->nbCandidat] = val;
        laCase->nbCandidat += 1;
    }
    else{
        while(laCase->candidats[i] < val){
            i++;    
        }
        for(int j = laCase->nbCandidat; j > i; j--){    
        laCase->candidats[j+1] = laCase->candidats[j];
        }
        laCase->candidats[i] = val;
        laCase->nbCandidat += 1;
    }
    
}

void retirerCandidat(tCase1 *laCase, int val,int nbCandidatsElimines){
    int i = 0;
    int valTemp;
    while(laCase->candidats[i] < val && i < TAILLE){
        i += 1;
    }
    laCase->candidats[i] = 0;
    for(int j = i; j < laCase->nbCandidat-1;j++){
        valTemp = laCase->candidats[j+1];
        laCase->candidats[j+1] = laCase->candidats[j];
        laCase->candidats[j] = valTemp;
    }
    laCase->nbCandidat -= 1;
    nbCandidatsElimines += 1;
}

bool estCandidat(tCase1 laCase, int val){
    int i = 0;
    while(laCase.candidats[i] < val){
        i += 1; 
    }
    if(laCase.candidats[i] == val){
        return true;
    }
    else{
        return false;
    }
}

int nbCandidat(tCase1 laCase){
    return laCase.nbCandidat;
}

void initialisationCandidats(tGrille g, float nbCandidatsTotal){
    for(int ligne = 0; ligne < TAILLE; ligne++){
        for(int colonne = 0; colonne < TAILLE; colonne++){
            g[ligne][colonne].nbCandidat = 0;
            if(g[ligne][colonne].valeur == 0){
                for(int i = 1;i<=TAILLE;i++){
                    ajouterCandidat(&g[ligne][colonne],i);
                    nbCandidatsTotal += 1;
                }
            }
        }
    }
}

void rechercheCandidats(tGrille *g,float nbCasesRemplies,float nbCandidatsElimines, float nbCasesVides){
    for(int ligne = 0; ligne < TAILLE; ligne++){
        for(int colonne = 0; colonne < TAILLE; colonne++){
            candidatsLigne(g,ligne,nbCandidatsElimines);
            candidatsColonne(g,colonne, nbCandidatsElimines);
            candidatsCarre(g,ligne%N,colonne%N,nbCandidatsElimines);
            if(g[ligne][colonne]->nbCandidat == 1){
                g[ligne][colonne]->valeur = g[ligne][colonne]->candidats[0];
                nbCasesRemplies += 1;
                nbCasesVides -= 1;
            }
        }
    }
}

void candidatsLigne(tGrille *g,int ligne, float nbCandidatsElimines){
    for(int i = 0;i<TAILLE;i++){
        if(g[ligne][i]->valeur != 0){
            for(int j = 0;j<TAILLE;j++){
                if(g[ligne][j]->valeur == 0){
                    retirerCandidat(g[ligne][j],g[ligne][i]->valeur,nbCandidatsElimines);
                }
            }
        }
    }
}

void candidatsColonne(tGrille *g,int colonne,float nbCandidatsElimines){
    for(int i = 0; i<TAILLE; i++){ 
        if(g[i][colonne]->valeur != 0){ 
            for(int j = 0; j < TAILLE; j++){ 
                if(g[j][colonne]->valeur == 0){
                    retirerCandidat(g[j][colonne],g[i][colonne]->valeur,nbCandidatsElimines);
                }
            }
        }
    } 
}

void candidatsCarre(tGrille *g, int ligne, int colonne,float nbCandidatsElimines){
    int ligneDepart = ligne * N-1;
    int colonneDepart = colonne * N-1;
    for(int i = ligneDepart;i<ligneDepart+N;i++){
        for(int j = colonneDepart;j<colonneDepart+N;j++){
            if(g[i][j]->valeur == 0){
                for(int l = ligneDepart;l<ligneDepart+N;l++){
                    for(int c = colonneDepart;c<colonneDepart+N;c++){
                        if(g[l][c]->valeur != 0){
                            retirerCandidat(g[i][j],g[l][c]->valeur,nbCandidatsElimines);
                        }
                    }
                }
            }
        }
    }
}

void afficherStats(char nomGrille[15], float nbCasesVides, float nbCasesRemplies, float nbCandidatsTotal, float nbCandidatsElimines){
    printf("******    RESULTAT POUR %s    ****** \n\n",nomGrille);
    printf("nombre de case remplies = %f sur %f \t",nbCasesRemplies,nbCasesVides);
    printf("Taux de remplissage =%.3f %%", 100.0*nbCasesRemplies/nbCasesVides);
    printf("nombre de candidats eliminés = %f \t", nbCandidatsElimines);
    printf("pourcentage d'élimination = %.3f %%\n\n", 100*nbCandidatsElimines/nbCandidatsTotal);
}   