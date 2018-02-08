/**
    M2 EEA Rob
    operations_flot_optique.hpp
    Purpose: Diverses fonctions d'opérations sur les matrices nécessaire au flot optique

    @author Tangi Pointeau et Albane Ordrenneau
    @version 1.1 07/02/2018
*/

#ifndef OPERATIONS_FLOT_OPTIQUE_H
#define OPERATIONS_FLOT_OPTIQUE_H

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include "operations_matrices.hpp"
#include "gradient_castan.hpp"

void mat2double(cv::Mat image, double *tableau, int lin, int col);
void double2mat(cv::Mat image, double *tableau, int lin, int col);
void construit_reference(cv::Mat image_in, double *matrice_reference, int lin, int col);
void normalisation(double *tableau_in, int lin, int col);
void affiche_cadre(cv::Mat image, int x, int y, int lin, int col);


#endif // OPERATIONS_FLOT_OPTIQUE_H
