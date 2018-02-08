/**
    M2 EEA Rob
    gradient_castan.hpp
    Purpose: Calcul du gradient d'une image par la méthode de Shen-Castan

    @author Tangi Pointeau et Albane Ordrenneau
    @version 2 07/02/2018
*/


#define FlecheHaut 2490368
#define FlecheBas 2621440
#define FlecheDroite 2555904
#define FlecheGauche 2424832
#define echap 27
#define espace 32
#define zKey 122
#define sKey 115
#define qKey 113
#define dKey 100
#define iKey 105
#define oKey 111
#define entree 13
#define Key1 49
#define Key2 50
#define Key3 51
#define esperluette 38
#define eaigu 233
#define guillemets 34
#define guillemet 39

#define ever ;;

#define NOM_FENETRE "Camera PTZ Hikvision"
#define NOM_PORT (char*)"COM1"
#define ADRESSE_CAM 0x1e

#define vitesseLente 0x1f
#define vitesseMoyenne 0x2f
#define vitesseRapide 0x3f

#define flin frame.rows
#define fcol frame.cols
#define mlin flin/4
#define mcol fcol/4
#define x_initial (flin-mlin)/2
#define y_initial (fcol-mcol)/2


