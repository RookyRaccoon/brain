/* L'algorithme SURF utilise des grandeurs comme les octaves, les intégrales,
les "box-filter" qui s'inspirent des filtres de Gauss, nous avons joint dans le rapport le tableau et
les explications pour les valeurs choisies correspondantes
on va donc fixer certaines conditions au bon développement de l'algo
A update au fur et a mesure pour balancer des nombres ou des trucs recurrents dans tout le prog*/

#ifndef SURFDEF
#define SURFDEF
#include<math.h>
#include<sstream>
#include<string>


typedef long int IMAGE_INTEGRALE;
typedef double IMAGE_NORMALE;

inline int arrondir(float flote);
inline double gaussien(double x, double y, double sigma);
inline std::string convertInt_Char(int i);
inline IMAGE_NORMALE val_absolue(IMAGE_NORMALE im);

#define pi 3.14159265358979323846



//ratio entre deux matchs
#define RATIO 0.6

//octave max
#define OCTAVE 4

//echelle l max (longueur)

#define LONGMAX 4

//nombre de sous régions max

#define NOMBRE_SOUS_REGIONS 20

//Echantillionage a chaque étape
#define SAMPLE_IMAGE 2

//Taille descriptors (i,j)
#define DESCRIPTOR_TAILLE_1D 4

//Fonction de gauss pour le box filter notamment (détaillé section 1.1 du rapport)

inline double gaussien(double x, double y, double sigma) {
	return 1 / (2 * pi*sigma*sigma)*exp(-(x*x + y * y) / (2 * sigma*sigma));
}

//fonction pour arrondir :
inline int arrondir(IMAGE_NORMALE im) { return (int)(im + 0.5f); }
inline int arrondir(float flote) { return (int)(flote + 0.5f); }


//fonction pour valeur absokue
inline IMAGE_NORMALE val_absolue(IMAGE_NORMALE im) { return ((im>0) ? im : -im); }

//conversion int->char

inline std::string convertInt_Char(int i)
{
	std::ostringstream oss;
	oss << i;
	return oss.str();
}


#endif // SURFDEF

#pragma once
