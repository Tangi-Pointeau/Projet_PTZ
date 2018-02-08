/**
    M2 EEA Rob
    operations_matrices.hpp
    Purpose: Diverses fonctions d'opérations générales sur les matrices

    @author Tangi Pointeau et Albane Ordrenneau
    @version 1.1 07/02/2018
*/

#ifndef OPERATIONS_MATRICES_H
#define OPERATIONS_MATRICES_H

#include <iostream>
#include <cmath>

void remplir_identite(double *matrice_in, int dim);
void copier_matrice(double *matrice_source, double *matrice_cible, int dim);
void transposer_matrice(double *matrice_in, double *matrice_out, int lin,int col);
void multiplier_matrices(double *matrice_in_gauche, double *matrice_in_droite, double *matrice_out, int lin1, int col1, int lin2, int col2);
void inverser_matrice(double *matrice_in, double *matrice_out, int dim);
void pseudo_inverser_matrice(double *matrice_in,double *matrice_out,int lin, int col);
double maximum_matrice(double *matrice_in, int lin, int col);
double minimum_matrice(double *matrice_in, int lin, int col);
void soustraire_matrices(double *matrice_in_gauche, double *matrice_in_droite, double *matrice_out, int lin, int col);

#endif //OPERATIONS_MATRICES_H
