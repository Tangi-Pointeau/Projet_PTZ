/**
    M2 EEA Rob
    Asservissement_PTZ/main.cpp
    Purpose: Asservissement visuel d'une caméra PTZ

    @author Tangi Pointeau et Albane Ordrenneau
    @version 1 08/02/2018
*/

#include <new>
#include <iostream>
#include <iomanip>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include "operations_matrices.hpp"
#include "operations_flot_optique.hpp"
#include "gradient_castan.hpp"
#include "pelco_d.hpp"
#include "main.hpp"

//Using namespace
using namespace std;
using namespace cv;

//Déclaration des variables
int code_clavier = -1;
Mat frame; //Image de la caméra
Mat motif; //Motif à suivre
int menu = 1;
int y_courant = y_initial;
int x_courant = x_initial;
char vitesse_pan = 0;
char vitesse_tilt = 0;
char sens_zoom = 0;

int main(){
    VideoCapture cap(1); // On ouvre la caméra d'ID 1 (0, c'est l'ID de la caméra par déafut de l'ordinateur
    if(!cap.isOpened()){  // On vérifie que l'ouverture a fonctionné
        cerr << "Impossible d'ouvrir la caméra" << endl;
        return -1;
    }
    //Initialisation de la communication caméra
    PelcoD pelco(ADRESSE_CAM,NOM_PORT);
    pelco.retour_position_initiale();
    namedWindow(NOM_FENETRE,WINDOW_AUTOSIZE);
    namedWindow("Motif",WINDOW_AUTOSIZE);

    //Ouverture de la référence
    cap >> frame; //Récupération de l'image courante
    flip(frame,frame,-1); //Retournement de l'image
    //Initialisation du motif
    motif = frame(Rect(y_initial, x_initial, mcol, mlin));
    double *matrice_motif = new double[mlin*mcol*2];
    double *matrice_imagette_precedente = new double[mlin*mcol];
    imshow("Motif",motif);
    construit_reference(motif,matrice_motif, mlin, mcol);


    //Boucle infinie
    for(ever){
        cap >> frame; //Récupération de l'image courante
        flip(frame,frame,-1); //Retournement de l'image
        Mat frame_d_affichage = frame.clone();
        affiche_cadre(frame_d_affichage, x_initial, y_initial, mlin, mcol);
        imshow(NOM_FENETRE,frame_d_affichage); //Affichage de l'image en cours
        cvtColor(frame,frame,COLOR_BGR2GRAY); // Mise en nuances de gris

        //Traitement de l'image
        if(menu == 0){ //Si on décide de réinitialiser le motif à suivre
            motif = frame(Rect(y_initial, x_initial, mcol, mlin));
            imshow("Motif",motif);
            construit_reference(motif,matrice_motif, mlin, mcol);
            x_courant = x_initial;
            y_courant = y_initial;
            menu = 1;
            vitesse_pan = 0;
            vitesse_tilt = 0;
            sens_zoom = 0;
        }else{ //Sinon on calcule le flot optique
            /*double *matrice_difference = new double[mlin*mcol];
            soustraire_matrices(double *matrice_in_gauche, double *matrice_in_droite, double *matrice_out, int lin, int col);
            double *vecteur_deplacement = new double[2];*/


        }

        //pelco.pan_tilt_zoom(vitesse_pan,vitesse_tilt,sens_zoom);

        //Contrôle clavier
        code_clavier = waitKey(25);
        if(code_clavier == echap){
            cout << "Demande de fin du programme..." << endl;
            pelco.stop();
            break;
        }else if(code_clavier == espace){
            cout << "Retour a la position initiale" << endl;
            pelco.retour_position_initiale();
            code_clavier = -1;
        }else if(code_clavier == entree){ //Tilt haut
            menu = 0;
            code_clavier = -1;
        }else if(code_clavier == FlecheHaut){ //Tilt haut
            pelco.pan_tilt_zoom(0,-vitesseMoyenne,0);
            code_clavier = -1;
        }else if(code_clavier == FlecheGauche){ //Pan gauche
            pelco.pan_tilt_zoom(-vitesseMoyenne,0,0);
            code_clavier = -1;
        }else if(code_clavier == FlecheDroite){ //Pan droite
            pelco.pan_tilt_zoom(vitesseMoyenne,0,0);
            code_clavier = -1;
        }else if(code_clavier == FlecheBas){ //Pan bas
            pelco.pan_tilt_zoom(0,vitesseMoyenne,0);
            code_clavier = -1;
        }else if(code_clavier == zKey){ //Zoom
            pelco.pan_tilt_zoom(0,0,1);
            code_clavier = -1;
        }else if(code_clavier == sKey){ //Dézoom
            pelco.pan_tilt_zoom(0,0,-1);
            code_clavier = -1;
        }else if(code_clavier == dKey){ //Focus loin
            pelco.focus(1);
            code_clavier = -1;
        }else if(code_clavier == qKey){ //Focus proche
            pelco.focus(-1);
            code_clavier = -1;
        }else{
            pelco.stop();
        }
    }

    return 0;
}
