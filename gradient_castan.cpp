/**
    M2 EEA Rob
    gradient_castan.hpp
    Purpose: Calcul du gradient d'une image par la méthode de Shen-Castan

    @author Tangi Pointeau et Albane Ordrenneau
    @version 2.0 07/02/2018
*/


#include "gradient_castan.hpp"


using namespace std;

/**
    Cette fonction utilise la méthode de Shen-Castan pour calculer le gradient d'une image.

    @param image_source Pointeur vers le tableau de doubles de taille lin*col contenant l'image source
    @param gradient_x Pointeur vers le tableau de doubles de taille lin*col où écrire le gradient en x
    @param gradient_y Pointeur vers le tableau de doubles de taille lin*col où écrire le gradient en y
    @param nlin Nombre de lignes de l'image source
    @param ncol Nombre de colonnes de l'image source
    @param alpha Paramètre de réglage de la qualité du gradient
*/
int gradient_castan(double *image_source,double *gradient_x,double *gradient_y,int nlin,int ncol,double alpha){
    //Variables locales
    double ao = ((alpha>0.0)&&(alpha<1.0)) ? alpha : 0.135; //ao = alpha si alpha est compris entre 0 et 1, sinon, ao = 0.135
    double oa = 1.0 - ao;
    double ko = oa/(2.0*ao);
    double klo= 1.0/(2.0-ao);
    int ncol1 = ncol - 1;
    int nlin1 = nlin - 1;
    int dim = (nlin>ncol) ? nlin : ncol; //dim est la taille du plus grand entre nlin et ncol
    int col, lin;
    double *B1,*B2, *dx, *dy;
    double *debut, *fin;
    double *pixel_suivant;
    double *B_courant, *B_suivant,*B_precedent;

    //allocation memoire
    B1  = ALLOCATION(dim,double);
    B2  = ALLOCATION(dim,double);
    if((B1==NULL)||(B2==NULL)){
        cout << "Impossible d'allouer B1 ou B2 dans la procedure de derivation" << endl;
        return(ALLOCATION_KO) ;
    }

    /**********************************/
    /* Calcul de la derivee suivant y */
    /**********************************/

    //lissage en x
    for(debut=image_source, lin=0; lin < nlin; lin++, debut+=ncol){
        //Lissage Causal : B1
        B_courant = B1;
        B_suivant = B1;
        pixel_suivant = debut;
        (*B_suivant) = (*pixel_suivant);
        B_suivant++;
        pixel_suivant++;
        fin = B1 + ncol;
        while(B_suivant < fin){
            (*B_suivant) = ao * (*B_courant) + oa * (*pixel_suivant);
            B_suivant++;
            B_courant++;
            pixel_suivant++;
        }

        //Lissage Anti-Causal : B2
        B_courant = B2 + ncol1;
        B_suivant = B2 + ncol1;
        pixel_suivant = debut + ncol1;
        (*B_suivant) =  (*pixel_suivant);
        B_suivant--;
        pixel_suivant--;
        fin = B2;
        while(B_suivant >= fin){
            (*B_suivant) = ao * (*B_courant) + oa * (*pixel_suivant);
            B_suivant--;
            B_courant--;
            pixel_suivant--;
        }

        B_precedent = B1;
        B_suivant = B2 ;
        dy = gradient_y + (lin*ncol);
        fin = dy + ncol;
        pixel_suivant = image_source + (lin * ncol);
        while(dy < fin){
            (*dy) = klo*((*B_precedent) + (*B_suivant)-ao * (*pixel_suivant)) ;
            dy++;
            B_precedent++;
            B_suivant++;
            pixel_suivant++;
        }
    } //fin du lissage

    //derivateur en y
    for(debut=gradient_y, col=0; col<ncol; col++, debut++){
        // derivateur Causal : B1
        B_courant = B1;
        B_suivant = B1;
        pixel_suivant = debut;
        (*B_suivant) =  (*pixel_suivant);
        B_suivant++;
        pixel_suivant+=ncol;
        fin = B1 + nlin;
        while(B_suivant < fin){
            (*B_suivant) = ao * (*B_courant) + oa * (*pixel_suivant);
            B_suivant++;
            B_courant++;
            pixel_suivant+=ncol;
        }

        // Lissage Anti-Causal : B2
        B_courant = B2 + nlin1;
        B_suivant = B2 + nlin1;
        pixel_suivant = debut + nlin1*ncol;
        (*B_suivant) = (*pixel_suivant);
        B_suivant--;
        pixel_suivant-=ncol;
        fin = B2;
        while(B_suivant>=fin){
            (*B_suivant) = ao * (*B_courant) + oa * (*pixel_suivant);
            B_courant--;
            B_suivant--;
            pixel_suivant-=ncol;
        }

        B_precedent = B1;
        B_suivant = B2;
        dy = gradient_y + col;
        fin = dy + nlin*ncol;
        while(dy<fin){
            (*dy) = ko*(-(*B_precedent) + (*B_suivant)) ;
            dy+=ncol;
            B_precedent++;
            B_suivant++ ;
        }
    } // fin du derivateur


    /**********************************/
    /* Calcul de la derivee suivant x */
    /**********************************/

    // lissage en y
    for(debut=image_source, col=0; col<ncol; col++, debut++){
        // lissage Causal : B1
        B_courant = B1;
        B_suivant = B1;
        pixel_suivant = debut;
        (*B_suivant) = (*pixel_suivant);
        B_suivant++;
        pixel_suivant+=ncol;
        fin = B1 + nlin;
        while(B_suivant < fin){
            (*B_suivant) = ao * (*B_courant) + oa * (*pixel_suivant);
            B_suivant++;
            B_courant++;
            pixel_suivant+=ncol;
        }

        // Lissage Anti-Causal : B2
        B_courant = B2 + nlin1;
        B_suivant = B2 + nlin1;
        pixel_suivant = debut + nlin1 *ncol;
        (*B_suivant) =  (*pixel_suivant);
        B_suivant--;
        pixel_suivant-=ncol;
        fin = B2;
        while(B_suivant >= fin){
            (*B_suivant) = ao * (*B_courant) + oa * (*pixel_suivant);
            B_suivant--;
            B_courant--;
            pixel_suivant-=ncol;
        }

        B_precedent = B1;
        B_suivant = B2;
        dx = gradient_x + col;
        fin = dx + nlin *ncol;
        pixel_suivant = image_source + col;
        while(dx < fin){
            (*dx) =klo*((*B_precedent) + (*B_suivant)-ao * (*pixel_suivant));
            dx+=ncol;
            B_precedent++;
            B_suivant++;
            pixel_suivant+=ncol;
        }
    } // fin du lissage

    // derivee en x
    for(debut=gradient_x, lin=0; lin<nlin; lin++, debut+=ncol){
        // derivee Causal : B1
        B_courant = B1;
        B_suivant = B1;
        pixel_suivant = debut;
        (*B_suivant) = (*pixel_suivant);
        B_suivant++;
        pixel_suivant++;
        fin = B1 + ncol;
        while(B_suivant < fin){
            (*B_suivant) = ao * (*B_courant) + oa * (*pixel_suivant);
            B_suivant++;
            B_courant++;
            pixel_suivant++;
        }
        // derivee Anti-Causal : B2
        B_courant = B2 + ncol1;
        B_suivant = B2 + ncol1;
        pixel_suivant = debut + ncol1;
        (*B_suivant) = (*pixel_suivant);
        B_suivant--;
        pixel_suivant--;
        fin = B2;
        while(B_suivant >= fin){
            (*B_suivant) = ao * (*B_courant) + oa * (*pixel_suivant);
            B_courant--;
            B_suivant--;
            pixel_suivant--;
        }

        B_precedent = B1;
        B_suivant = B2;
        dx = gradient_x + (ncol*lin);
        fin = dx + ncol;
        while(dx < fin){
            (*dx) =ko*(-(*B_precedent) + (*B_suivant));
            dx++;
            B_precedent++;
            B_suivant++;
        }
    } // fin du derivateur
    DESALLOCATION(B1);
    DESALLOCATION(B2);
    return TERMINE; // tout s'est bien passé.
}
