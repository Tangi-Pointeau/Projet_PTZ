/**
    M2 EEA Rob
    pelco_d.hpp
    Purpose: Protocole de commande Pelco_D pour une caméra PTZ via communication USB

    @author Tangi Pointeau et Albane Ordrenneau
    @version 1.4 07/02/2018
*/


#ifndef ASSERVISSEMENT_PELCOD_H
#define ASSERVISSEMENT_PELCOD_H

#include <windows.h>
//#include <stdint.h>

#include <iostream>
#include <string>
#include <cmath>
#include <cstdint>

//Sur Commande 1 (data[2])
#define bitFocusProche 0b00000001
//Sur Commande 2 (data[3])
#define bitFocusLoin 0b10000000
#define bitDezoom 0b01000000
#define bitZoom 0b00100000
#define bitTiltBas 0b00010000
#define bitTiltHaut 0b00001000
#define bitPanGauche 0b00000100
#define bitPanDroite 0b00000010


/**
    Protocole de communication Pelco_D via connexion USB

    Exemple :
            PelcoD pelco(ADRESSE_CAM, NOM_PORT);
            pelco.pan_tilt_zoom(0x3f, 0x3f, -1);
            sleep(1000);
            pelco.stop();
*/
class PelcoD{
	private:
	    //Variables privées
		uint8_t data[7]; //Octets de commande Pelco_D
		char sendbuffer[7]; // Buffer d'envoi de données USB
		HANDLE h; // Handle de la connexion USB
		char* portname; // Nom du port USB (paramètre du constructeur)
		int baudrate; // Vitesse de comminication en Baud (9600 par défaut)
		//Fonctions privées
		void clear_data(); //Fonction qui nettoie les octets 3 à 7 de la commande
		void ouvre_port_serie(); //Fonction qui oubre la connexion série
		void error_exit(std::string code_erreur); //Fonction d'erreur de la connexion USB
		void calcule_checksum(); //Fonction de calcul du dernier octet de la commande
		void envoie_commande(); //Fonction qui envoie la commande actuelle à la caméra
	public:
		PelcoD(uint8_t adresse,char* portname);//Constructeur
		~PelcoD();//Destructeur
		//Fonctions publiques
		void pan_tilt_zoom(char vitesse_pan, char vitesse_tilt, char sens_zoom);  //Fonction qui règle la vitesse pan, tilt et zoom de la caméra
		void retour_position_initiale(); //Fonction qui renvoie la caméra à sa position initiale
		void focus(char sens); //Fonction qui regle le focus
		void preset(uint8_t preset); //Fonction qui regle les preset
		void stop(); //Fonction qui arrête la caméra
};



#endif // ASSERVISSEMENT_PELCOD_H
