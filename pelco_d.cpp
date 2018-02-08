/**
    M2 EEA Rob
    pelco_d.cpp
    Purpose: Protocole de commande Pelco_D pour une cam�ra PTZ via communication USB

    @author Tangi Pointeau et Albane Ordrenneau
    @version 1.4 07/02/2018
*/

#include "pelco_d.hpp"

using namespace std;

/**
    Constructeur de la classe
    Le constructeur initialise les deux premiers octets de commande, la vitesse de communication et ouvre le port s�rie

    @param adresse Adresse de la cam�ra fix�e avec les DIP switch
    @param portname Nom du port de communication (id�alement "COM1")
*/
PelcoD::PelcoD(uint8_t adresse,char* portname){
    this->portname=portname;
	clear_data(); //Nettoyage de la commande
	data[0] = 0xFF; //Octet de synchronisation
	data[1] = adresse; //Octet d'adresse
    baudrate = 9600; //Baudrate par d�faut
    ouvre_port_serie(); //Ouverture de la connection
    retour_position_initiale();
    stop();
}


/**
    Destructeur de la classe
    Le destructeur remet la cam�ra en position initiale puis femer la connexion USB
*/
PelcoD::~PelcoD(){
	cout << "Retour de la camera en position initiale..." << endl;
	retour_position_initiale();
	cout << "Fermeture de la connexion USB..." << endl;
	stop();
	CloseHandle(h);
}



/**
    Nettoyage de data
    Cette fonction remet les octets 3 � 7 (data[2] � data[6]) � 0.
    Envoyer � la cam�ra une commande nettoy�e avec clear_data() arr�te tout mouvement de la cam�ra
*/
void PelcoD::clear_data(){
	for(int i=2;i<7;i++){
        data[i]=0;
	}
}


/**
    Fonction priv�e : ouvre le port s�rie.
    Cette fonction ouvre le port s�rie d�fini lors de la construction de l'objet. Elle est automatiquement appel�e lors de la construction.
*/
void PelcoD::ouvre_port_serie(){
	DWORD  accessdirection = GENERIC_WRITE;
	h = CreateFile(portname,
                    accessdirection,
                    0,
                    0,
                    OPEN_EXISTING,
                    0,
                    0);
	if (h == INVALID_HANDLE_VALUE) {
		error_exit("CreateFile");
	}
	DCB dcbSerialParams = {0};
	dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
	if (!GetCommState(h, &dcbSerialParams)) {
		 error_exit("GetCommState");
	}
	dcbSerialParams.BaudRate=baudrate;
	dcbSerialParams.ByteSize=8;
	dcbSerialParams.StopBits=ONESTOPBIT;
	dcbSerialParams.Parity=NOPARITY;
	if(!SetCommState(h, &dcbSerialParams)){
		 error_exit("SetCommState");
	}
	COMMTIMEOUTS timeouts={0};
	timeouts.ReadIntervalTimeout=50;
	timeouts.ReadTotalTimeoutConstant=50;
	timeouts.ReadTotalTimeoutMultiplier=10;
	timeouts.WriteTotalTimeoutConstant=50;
	timeouts.WriteTotalTimeoutMultiplier=10;
	if(!SetCommTimeouts(h, &timeouts)){
		error_exit("SetCommTimeouts");
	}
}



/**
    Fonction priv�e : g�n�re une fen�tre d'erreur si la connexion USB �choue
    La fonction est priv�e car elle n'est appel�e que dans la fonction ouvre_port_serie()
*/
void PelcoD::error_exit(string code_erreur){
    // Retrieve the system error message for the last-error code
    char *lpszFunction = &code_erreur[0u];//Conversion en char* pour utilisation
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );
    // Display the error message and exit the process
    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
    (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    sprintf((LPTSTR)lpDisplayBuf, TEXT("%s failed with error %d:\n%s"), lpszFunction, (int)dw, (char*)lpMsgBuf);
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);
    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    ExitProcess(dw);
}

/**
    Fonction priv�e : calcule le checksum de l'octet 7 de commande
    La fonction est priv�e car elle n'est appel�e que dans la fonction envoie_commande()
    Le checksum est la somme des octets 2 � 6 (data[1] � data[5]) modulo 256
*/
void PelcoD::calcule_checksum(){
	uint16_t checksum = 0;
	for(int i = 1; i < 6; ++i){
		checksum += data[i];
	}
	checksum = checksum % 256;
	data[6] = (uint8_t)checksum;
}


/**
    Fonction priv�e : envoie la commande de data � la cam�ra par le port USB
    La fonction est priv�e car elle n'est appel�e que dans les autres fonctions de commnde tel que pan_tilt_zoom
*/
void PelcoD::envoie_commande(){
	calcule_checksum();
	for(int i=0;i<7;i++){
        sendbuffer[i]=data[i];
	}
	unsigned long p=0;
	if(!WriteFile(h, sendbuffer, 7, &p, NULL)){
		error_exit("WriteFile");
	}
}


/**
    Fonction publique : r�glage pan, tilt et zoom de la cam�ra
    La fonction prend en param�tre les vitesse pan, tilt et zoom, g�n�re la commande et l'envoie � la cam�ra

    @param vitesse_pan : vitesse de rotation autour de l'axe vertical (-0x3f � 0x3f)
    @param vitesse_tilt : vitesse de rotation autour de l'axe horizontal (-0x3f � 0x3f)
    @param sens_zoom : zoom avant/arri�re (-1 arri�re, 1 avant, 0 pas de zoom)
*/
void PelcoD::pan_tilt_zoom(char vitesse_pan, char vitesse_tilt, char sens_zoom){
	clear_data();
	//Reglage du pan
	if(vitesse_pan < 0){ //pan gauche
		data[3] &= ~bitPanDroite;
		data[3] |= bitPanGauche;
	}else{ //pan droite
		data[3] &= ~bitPanGauche;
		data[3] |= bitPanDroite;
	}
    vitesse_pan = abs(vitesse_pan);
    if(vitesse_pan > 0x3f){ //Limitation de la vitesse � 3f (selon la doc)
		vitesse_pan = 0x3f;
	}
	data[4] = vitesse_pan;

	//Reglage du tilt
    if(vitesse_tilt < 0){ //tilt bas
		data[3] &= ~bitTiltHaut;
		data[3] |= bitTiltBas;
	}else{ //tilt haut
		data[3] &= ~bitTiltBas;
		data[3] |= bitTiltHaut;
	}
    vitesse_tilt = abs(vitesse_tilt);
    if(vitesse_tilt > 0x3f){ //Limitation de la vitesse � 3f (selon la doc)
		vitesse_tilt = 0x3f;
	}
	data[5] = vitesse_tilt;

    //Reglage du zoom
    if(sens_zoom < 0){ //Zoom arri�re
		data[3] &= ~bitZoom;
		data[3] |= bitDezoom;
	}else if(sens_zoom >0){ //Zoom avant
		data[3] &= ~bitDezoom;
		data[3] |= bitZoom;
	}else{ //Zoom nul
		data[3] &= ~bitDezoom;
		data[3] &= ~bitZoom;
	}

	//Envoi de la commande
	envoie_commande();
}


/**
    Fonction publique : retour de la cam�ra en position initiale
    Cette fonction r�gle le preset pour renvoyer la cam�ra � sa position initiale.
*/
void PelcoD::retour_position_initiale(){
    data[2] = 0;
	data[3] = 0x07;
	data[4] = 0;
	data[5] = 34;
	envoie_commande();
}



/**
    Fonction publique : r�glage du focus de la cam�ra

    @param sens : sens du focus (-1 focus proche, 1 focus loin, 0 pas de focus)
*/
void PelcoD::focus(char sens){
	if(sens > 0){ //focus loin
		data[2] &= ~bitFocusProche;
		data[3] |= bitFocusLoin;
	}else if(sens < 0){ //focus proche
		data[3] &= ~bitFocusLoin;
		data[2] |= bitFocusProche;
	}else{ //Focus nul
		data[2] &= ~bitFocusProche;
		data[3] &= ~bitFocusLoin;
	}
}



/**
    Fonction publique : r�glage des preset de la cam�ra
    /!\ Il vaut mieux �viter de l'utiliser pour l'instant

    @param preset : valeur du preset � mettre
*/
void PelcoD::preset(uint8_t preset){
	data[2] = 0;
	data[3] = 0x07;
	data[4] = 0;
	if(preset > 0xFF){
		preset = 0xFF; //only 20 presets allowed according to doc
	}
	data[5] = preset;
}



/**
    Fonction publique : arr�t des op�rations de la cam�ra
*/
void PelcoD::stop(){
    clear_data();
	envoie_commande();
}
