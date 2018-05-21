/*Point clés (keypoints) une des parties les plus importantes du projet
on aura comme attribut : les coordonnes, l'échelle l et l'orientation.
*/

#ifndef KEYPOINT
#define KEYPOINT

#include <vector>
#include <math.h>

#include "surfdef.h"
#include "image.h"
#include "integrale.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <ctime>


// Classe points cles
class keyPoint {
public:
	double x, y, scale, orientation;
	bool signLaplacian;         //pour les descripteurs
								// Constructeur
	keyPoint(IMAGE_NORMALE x_, IMAGE_NORMALE  y_, IMAGE_NORMALE  scale_, IMAGE_NORMALE  orientation_, bool signLaplacian_) :x(x_), y(y_), scale(scale_), orientation(orientation_), signLaplacian(signLaplacian_) {}
	keyPoint() {}
};

//liste de points cles
typedef  std::vector<keyPoint*> listKeyPoints;


#include "descriptor.h"

// On crée un point clé :
// (i,j) les coordonnées, signLapl est le signe du laplacien d=(1/L^2)*(Dxx+Dyy)*u où Dxx determinant du second ordre, u l'image normale
// et l'échelle (scale) est la taille de la "box"
//Le signe du laplacien est également reutilisé pour écremer les recherches de match
void addKeyPoint(imageIntegrale* img, IMAGE_NORMALE  i, IMAGE_NORMALE  j, bool signLapl, IMAGE_NORMALE  scale, listKeyPoints* listKeyPoints);

// Orientation du point clé (avec box filter de premier ordre). L'orientation du point clé est définie comme le max local de la fonction qui somme les orientations
float getOrientation(imageIntegrale* imgInt, int x, int y, int sectors, IMAGE_NORMALE  scale);

// On rejette ou on interpole les coordonnées du pt clé necessaire car on a echantillonné image
bool interpolationScaleSpace(image** img, int x, int y, int i, IMAGE_NORMALE  &x_, IMAGE_NORMALE  &y_, IMAGE_NORMALE  &s_, int sample, int octaveValue);


// Verifier si un point est un max et au dessus du seuil (threshold dans le tableau du rapport) le th=10^3
inline bool isMaximum(image** imageStamp, int x, int y, int scale, float threshold)
{
	IMAGE_NORMALE  tmp = (*(imageStamp[scale]))(x, y);

	if (tmp>threshold)
	{
		for (int j = -1 + y; j<2 + y; j++)
			for (int i = -1 + x; i<2 + x; i++) {
				if ((*(imageStamp[scale - 1]))(i, j) >= tmp)
					return false;
				if ((*(imageStamp[scale + 1]))(i, j) >= tmp)
					return false;
				if ((x != i || y != j) && (*(imageStamp[scale]))(i, j) >= tmp)
					return false;
			}
		return true;
	}
	else
		return false;
}
#endif



#pragma once
