/*Classe image  on va beaucoup réutiliser donc ifndef
Il s'agit de creer une classe pour les premieres manipulations de l'image
Les attributs sont donc centrés sur la taille de l'image, qui sont des parametres dans tous
les calculs qui seront réalisés par la suite (logique car la théorie repose que le calcul d'intégrale,
de manipulation de filtre de gauss qui prend cette taille en parametre, du Laplacien, etc */


#ifndef IMAGE
#define IMAGE

#include "surfdef.h"
#include <string.h>
#include "C:\Users\edeha\Documents\ECOLE\s4\Brainmatch\BrainMatch\fichiers_externes/io_png.h"

//classe image QUE pour les images normales

class image {
public: //constructeurs et destructeurs
	image(int x, int y);
	image(int x, int y, unsigned char* data);
	image(image *im);
	~image() { delete[] img; }
	//accesseur:
	inline int get_largeur() { return largeur; }
	inline int get_hauteur() { return hauteur; }
	//methodes
	inline IMAGE_NORMALE operator()(int x, int y) const { return img[y*largeur + x]; }
	inline IMAGE_NORMALE& operator()(int x, int y) { return img[y*largeur + x]; }

	// Afficher 3 images avec leurs matchs avec ORSA.
	void printImagePara(char fileName[], image* para);

	// On se place dans l'intervalle [0;255] pour la normalisation de l'image
	void normalizeImage();

	// Taille de l'image échantillonnée (largeur, hauteur)
	void getSampledImage(int& l, int& h, int sample);

	// Marge de l'image:
	image* marImage(int marge);


private:
	int	largeur, hauteur; // taille
	IMAGE_NORMALE*	img; // pointeur qui contient image


};


// On crée une classe spéciale pour les images intégrées
class imageIntegrale {
public:
	// constructeurs et destructeurs.
	imageIntegrale(image* im);
	~imageIntegrale();

	// Computation de l'image intégrale
	void computeImageIntegrale(image* image);
	inline IMAGE_INTEGRALE& operator()(int x, int y) { return img[largeur*(y + marge) + (x + marge)]; } // fixe taille
	inline IMAGE_INTEGRALE  operator()(int x, int y) const { return img[largeur*(y + marge) + (x + marge)]; } // accesseur


private:
	int	largeur, hauteur;	// taille de l'image intégrale
	IMAGE_INTEGRALE* img; // taille
	int marge; // de l'image normale
};

#endif // IMAGE
#pragma once
