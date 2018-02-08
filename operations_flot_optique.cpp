/**
    M2 EEA Rob
    operations_flot_optique.cpp
    Purpose: Diverses fonctions d'opérations sur les matrices nécessaire au flot optique

    @author Tangi Pointeau et Albane Ordrenneau
    @version 1.1 07/02/2018
*/

#include "operations_flot_optique.hpp"

using namespace cv;

/**
    Copie des valeurs de pixels d'une image openCV Mat dans un tableau de doubles

    @param image Image de type Mat à copier
    @param *tableau Pointeur vers le tableau de doubles de taille lin*col où copier les pixels
    @param lin Nombre de lignes dans l'image à copier
    @param col Nombre de colonnes dans l'image à copier
*/
void mat2double(Mat image, double *tableau, int lin, int col){
    for(int i=0; i<lin; i++){
        for(int j=0; j<col;j++){
                tableau[i*col+j] = image.data[image.step[0]*i + image.step[1]*j];
        }
    }
}


/**
    Copie des valeurs d'un tableau de doubles dans les pixels d'une image openCV Mat

    @param image Image de type Mat dans laquelle copier les valeurs
    @param *tableau Pointeur vers le tableau de doubles de taille lin*col dont on copie les valeurs
    @param lin Nombre de lignes dans l'image dans laquelle copier
    @param col Nombre de colonnes dans l'image dans laquelle copier
*/
void double2mat(Mat image, double *tableau, int lin, int col){
    for(int i = 0; i < lin; i++){
        for(int j = 0; j < col; j++){
            image.data[image.step[0]*i + image.step[1]*j+2] = tableau[i*col+j];
            image.data[image.step[0]*i + image.step[1]*j+1] = tableau[i*col+j];
            image.data[image.step[0]*i + image.step[1]*j]   = tableau[i*col+j];
        }
    }
}


/**
    Normalise le contenu d'un tableau

    @param tableau_in Pointeur vers le tableau à normaliser
    @param lin Nombre de lignes dans le tableau
    @param col Nombre de colonnes dans le tableau
*/
void normalisation(double *tableau_in, int lin, int col){
    double minimum = minimum_matrice(tableau_in,lin,col);
    double maximum = maximum_matrice(tableau_in,lin,col);
    for(int i = 0; i < lin; i++){
        for(int j = 0; j < col; j++){
            tableau_in[i*col+j]= 255*(tableau_in[i*col+j]-minimum)/(maximum-minimum);
        }
    }
}


/**
    Remplit la matrice pseudo-inverse des gradients pour le flot optique

    @param image Motif à suivre
    @param *matrice_reference Matrice pseudo-inverse des gradients du motif de référence
    @param lin Nombre de lignes dans le motif
    @param col Nombre de colonnes dans le motif
*/
void construit_reference(Mat image_in, double *matrice_reference, int lin, int col){
    double *motif_double = new double[lin*col];
    double *gradient_x = new double[lin*col];
    double *gradient_y = new double[lin*col];
    mat2double(image_in, motif_double, lin, col);
    gradient_castan(motif_double, gradient_x, gradient_y, lin, col, 100);
    normalisation(gradient_x, lin, col);
    normalisation(gradient_y, lin, col);
    for(int i=0; i<(lin*col);i++){
        matrice_reference[i*2]=gradient_x[i];
        matrice_reference[i*2+1]=gradient_y[i];
    }
    pseudo_inverser_matrice(matrice_reference, matrice_reference, lin, col);
    delete[] motif_double;
    delete[] gradient_x;
    delete[] gradient_y;
}


/**
    Ajoute un cadre rouge sur l'image en paramètre

    @param image Image au format Mat sur laquelle rajouter le cadre
    @param x Coordonnée de la colonne du coin haut-gauche du cadre
    @param y Coordonnée ligne du coin haut-gauche du cadre
    @param lin Taille verticale du cadre
    @param col Taille horizontale du cadre
*/
void affiche_cadre(cv::Mat image, int x, int y, int lin, int col){
    //Ligne du haut
    for(int i=0; i<col; i++){
        image.data[image.step[0]*x + image.step[1]*(y+i)+2] = 255;
        image.data[image.step[0]*x + image.step[1]*(y+i)+1] = 0;
        image.data[image.step[0]*x + image.step[1]*(y+i)]   = 0;
    }
    //Ligne du bas
    for(int i=0; i<col; i++){
        image.data[image.step[0]*(x+lin) + image.step[1]*(y+i)+2] = 255;
        image.data[image.step[0]*(x+lin) + image.step[1]*(y+i)+1] = 0;
        image.data[image.step[0]*(x+lin) + image.step[1]*(y+i)]   = 0;
    }
    //Ligne de gauche
    for(int i=0; i<lin; i++){
        image.data[image.step[0]*(x+i) + image.step[1]*y+2] = 255;
        image.data[image.step[0]*(x+i) + image.step[1]*y+1] = 0;
        image.data[image.step[0]*(x+i) + image.step[1]*y]   = 0;
    }
    //Ligne de gauche
    for(int i=0; i<lin; i++){
        image.data[image.step[0]*(x+i) + image.step[1]*(y+col)+2] = 255;
        image.data[image.step[0]*(x+i) + image.step[1]*(y+col)+1] = 0;
        image.data[image.step[0]*(x+i) + image.step[1]*(y+col)]   = 0;
    }
}

