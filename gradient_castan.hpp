/**
    M2 EEA Rob
    gradient_castan.hpp
    Purpose: Calcul du gradient d'une image par la méthode de Shen-Castan

    @author Tangi Pointeau et Albane Ordrenneau
    @version 2 07/02/2018
*/

#include <cmath>
#include <iostream>

#ifndef GRADIENT_CASTAN_H
#define GRADIENT_CASTAN_H

#define ALLOCATION(N,type) new type [N]
#define DESALLOCATION(pointeur) delete pointeur

#define ALLOCATION_KO 0
#define ALLOCATION_OK 1
#define TERMINE 2

#define EPSILON 1e-32

#define Fabs(a) ( ( (a) > 0.0 ) ? (a) : (-(a)) )
#define Min(a,b) ( ( (a) < (b) ) ? (a) : (b) )
#define Max(a,b) ( ( (a) > (b) ) ? (a) : (b) )


int gradient_castan(double *image_source, //Pointeur vers l'image source
                    double *gradient_x, //Pointeur vers le tableau où stocker le gradient en X
                    double *gradient_y, //Pointeur vers le tableau où stocker le gradient en y
                    int nlin, //Nombre de ligne de l'image source
                    int ncol, //Nombre de colonnes de l'image source
                    double alpha);

#endif //GRADIENT_CASTAN_H

