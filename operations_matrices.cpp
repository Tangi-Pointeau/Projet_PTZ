/**
    M2 EEA Rob
    operations_matrices.cpp
    Purpose: Diverses fonctions d'opérations générales sur les matrices

    @author Tangi Pointeau et Albane Ordrenneau
    @version 1.1 07/02/2018
*/

#include "operations_matrices.hpp"

using namespace std;

/**
    Remplissage d'une matrice identité

    @param *matrice_in Pointeur vers le tableau de doubles à remplir de taille dim*dim
    @param dim Dimension de la matrice à remplir
*/
void remplir_identite(double *matrice_in, int dim){
    for(int i=0; i<dim; i++){
        for(int j=0; j<dim; j++){
            if(i==j){
                matrice_in[i+j*dim] = 1;
            }else{
                matrice_in[i+j*dim] = 0.;
            }
        }
    }
}


/**
    Cette fonction permet de recopier le contenu d'une matrice carrée dans une autre

    @param *matrice_source Pointeur vers la matrice à recopier de taille dim*dim
    @param *matrice_cible Pointeur vers la matrice dans laquelle recopier de taille dim*dim
    @param dim Dimension de la matrice à recopier
*/
//fonction qui copie une matrice dans une autre
void copier_matrice(double *matrice_source, double *matrice_cible, int dim){
	for(int i=0; i<dim; i++){
		for(int j=0; j<dim; j++){
			matrice_cible[i+j*dim] = matrice_source[i+j*dim];
		}
	}
}


/**
    Cette fonction permet de transposer une matrice

    @param *matrice_in Pointeur vers la matrice à transposer de taille lin*col
    @param *matrice_out Pointeur vers la matrice transposée de taille lin*col
    @param lin Nombre de lignes dans la matrice à transposer
    @param col Nombre de colonnes dans la matrice à transposer
*/
void transposer_matrice(double *matrice_in, double *matrice_out, int lin,int col){
    for(int i=0; i<lin; i++){
        for(int j=0; j<col; j++){
            matrice_out[j*lin+i] = matrice_in[i*col+j];
        }
    }
}


/**
    Cette fonction permet de faire le produit de deux matrices
    Si les dimensions des deux matrices de correspondent pas, la matrice de sortie sera mise à 0

    @param *matrice_in_gauche Pointeur vers la matrice à gauche du produit  de taille lin1*col1
    @param *matrice_in_droite Pointeur vers la matrice à droite du produit  de taille lin2*col2
    @param *matrice_out Pointeur vers la matrice où stocker le résultat du produit  de taille col1*lin2
    @param lin1 Nombre de lignes dans la matrice à gauche du produit
    @param col1 Nombre de colonnes dans la matrice à gauche du produit
    @param lin2 Nombre de lignes dans la matrice à droite du produit
    @param col2 Nombre de colonnes dans la matrice à droite du produit
*/
void multiplier_matrices(double *matrice_in_gauche, double *matrice_in_droite, double *matrice_out, int lin1, int col1, int lin2, int col2){
    // Initialisation de la matrice de sortie
	for(int i = 0; i < lin1; ++i){
		for(int j = 0; j < col2; ++j){
			matrice_out[i*col2+j] = 0.;
		}
	}
	//Vérification de la taille des matrices
    if(col1!=lin2){
        cout << "Produit impossible, les dimensions sont incorrectes" << endl;
        cout << "Dimension matrice 1 : " << lin1 << "*" << col1 << endl;
        cout << "Dimension matrice 2 : " << lin2 << "*" << col2 << endl;
        cout << "ncol1 doit être égal à nlin2" << endl;
    }else{
        //Multiplication des matrices
        for(int i = 0; i < lin1; ++i){
            for(int j = 0; j < col2; ++j){
                for(int k=0; k<col1; ++k){
                    matrice_out[i*col2+j] += matrice_in_gauche[i*col1+k] * matrice_in_droite[k*col2+j];
                }
            }
        }
    }
}



/**
    Cette fonction permet d'inverser une matrice carrée

    @param *matrice_in Pointeur vers la matrice à inverser  de taille dim*dim
    @param *matrice_out Pointeur vers la matrice inverse de taille dim*dim
    @param dim Dimension de la matrice carrée
*/
//la matrice mat1 est celle qui est à inverser
//et l'inverse est contenu dans mat2
void inverser_matrice(double *matrice_in, double *matrice_out, int dim){
    double *temp = new double[dim*dim]; //Matrice locale pour éviter d'écraser celle en entrée
    copier_matrice(matrice_in, temp, dim);
    double a = 0.;
    double b = 0.;
    int c = 0;
    remplir_identite(matrice_out, dim);
    for(int k=0; k<dim; k++){
        a = temp[k+k*dim];
        c = 0;
        while(abs(a) < 0.000000001){
            c++;
            for(int q=0; q<dim; q++){
                temp[k+q*dim] = temp[k+q*dim] + temp[k+c+q*dim];
                matrice_out[k+q*dim] = matrice_out[k+q*dim] + matrice_out[k+c+q*dim];
            }
            a = temp[k+k*dim];
        }
        //normalisation de la ligne k
        for(int l=0; l<dim; l++){
            temp[k+l*dim] = temp[k+l*dim]/a;
            matrice_out[k+l*dim] = matrice_out[k+l*dim]/a;
        }
        //reduction de gauss-jordan
        for(int i=0; i<dim; i++){
            b = temp[i+k*dim];
            if(i != k){
                for(int j=0; j<dim; j++){
                    temp[i+j*dim] = temp[i+j*dim] - b*temp[k+j*dim];
                    matrice_out[i+j*dim] = matrice_out[i+j*dim] - b*matrice_out[k+j*dim];
                }
            }
        }
    }
    delete[] temp;
}



/**
    Cette fonction permet d'inverser une matrice cnon carrée en calculant la pseudo-inverse de Penrose-Moore
    Telle que pinv(a) = inv(trans(A)*A)*trns(A)

    @param *matrice_in Pointeur vers la matrice à inverser  de taille lin*col
    @param *matrice_out Pointeur vers la matrice pseudo-inverse de taille col*lin
    @param lin Nombre de lignes dans la matrice à inverser
    @param col Nombre de colonnes dans la matrice à inverser
*/
//Fonction qui fait la pseudo-inverse de Penrose-Moore d'une matrice
void pseudo_inverser_matrice(double *matrice_in,double *matrice_out,int lin, int col){
    if(lin==col){
        inverser_matrice(matrice_in, matrice_out, lin);
    }else{
        double *trans = new double[col*lin]; //Matrice locale contenant trans(A)
        transposer_matrice(matrice_in, trans,lin, col);
        double *prod = new double[col*col]; //Matrice locale contenant trans(A)*A
        multiplier_matrices(trans, matrice_in, prod, col, lin, lin, col);
        inverser_matrice(prod, prod, col);
        multiplier_matrices(prod, trans, matrice_out, col, col, col, lin);
        delete[] trans;
        delete[] prod;
    }
}


/**
    Trouve le maximum dans une matrice

    @param *matrice_in Pointeur vers la matrice dans laquelle chercher le maximum
    @param lin Nombre de lignes de la matrice
    @param col Nombre de colonnes de la matrice
    @return La valeur du maximum trouvée dans le tableau
*/
double maximum_matrice(double *matrice_in, int lin, int col){
    double maximum = 0;
    for(int i = 0; i < lin; i++){
        for(int j = 0; j < col; j++){
            if(matrice_in[i*col+j]>maximum){
                maximum = matrice_in[i*col+j];
            }
        }
    }
    return maximum;
}


/**
    Trouve le minimum dans une matrice

    @param *matrice_in Pointeur vers la matrice dans laquelle chercher le minimum
    @param lin Nombre de lignes de la matrice
    @param col Nombre de colonnes de la matrice
    @return La valeur du minimum trouvée dans le tableau
*/
double minimum_matrice(double *matrice_in, int lin, int col){
    double minimum = 0;
    for(int i = 0; i < lin; i++){
        for(int j = 0; j < col; j++){
            if(matrice_in[i*col+j]>minimum){
                minimum = matrice_in[i*col+j];
            }
        }
    }
    return minimum;
}


/**
    Effectue la différence de deux matrices

    @param *matrice_in_gauche Pointeur vers la matrice à gauche de la soustraction  de taille lin*col
    @param *matrice_in_droite Pointeur vers la matrice à droite de la soustraction  de taille lin*col
    @param *matrice_out Pointeur vers la matrice où stocker le résultat de la soustraction de taille lin*col
    @param lin Nombre de lignes dans les matrices de la soustraction
    @param col Nombre de colonnes dans les matrices de la soustractiont
*//*
void soustraire_matrices(double *matrice_in_gauche, double *matrice_in_droite, double *matrice_out, int lin, int col){
    // Initialisation de la matrice de sortie
	for(int i = 0; i < lin; ++i){
		for(int j = 0; j < col; ++j){
			matrice_out[i*col+j] = 0.;
		}
	}
	//Soustraction des matrices
    for(int i = 0; i < lin1; ++i){
        for(int j = 0; j < col2; ++j){
            matrice_out[i*col+j] = matrice_in_gauche[i*col1+j] - matrice_in_droite[i*col+j];
        }
    }
}*/
