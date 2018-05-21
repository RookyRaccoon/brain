/* Classe des descripteurs
un vecteur 16x4 qui �quivaut au gradient normalis� d'une grille R divis�e en sous r�gions 4x4
on obtient le descriptor final en concat�nant les 16 vecteurs obtenus */

#ifndef DESCRIPTOR

#define DESCRIPTOR
#include <math.h>
#include "image.h"
#include "keypoint.h"
#include "surfdef.h"



// Classe du vecteur descripteur pout le point cl� detect� ce vecteur �(i,j) sera ensuite concat�n� aux autre pour donner le descriptor final
class vectorDescriptor {
public:
	IMAGE_NORMALE sumDx; //somme des gradients locaux des sous r�gions
	IMAGE_NORMALE sumDy;
	IMAGE_NORMALE sumAbsDx; //meme somme en valeur absolue
	IMAGE_NORMALE sumAbsDy;
	// Init
	vectorDescriptor(IMAGE_NORMALE sumDx_, IMAGE_NORMALE sumDy_, IMAGE_NORMALE sumAbsDx_, IMAGE_NORMALE sumAbsDy_) :sumDx(sumDx_), sumDy(sumDy_), sumAbsDx(sumAbsDx_), sumAbsDy(sumAbsDy_) {}
	vectorDescriptor() :sumDx(0), sumDy(0), sumAbsDx(0), sumAbsDy(0) {}
};



// Descriptor
class descriptor {
public:

	vectorDescriptor * list;

	keyPoint *kP;// pointeur vers le point cl�
				 //const
	descriptor() { list = new vectorDescriptor[16]; } //taille 16 vecteurs obtenus
													  //dest
	~descriptor() { delete kP; delete[] list; }

	// copie constructor
	descriptor(const descriptor & des) {
		list = new vectorDescriptor[DESCRIPTOR_TAILLE_1D*DESCRIPTOR_TAILLE_1D];
		for (int i = 0; i<DESCRIPTOR_TAILLE_1D*DESCRIPTOR_TAILLE_1D; i++)
			list[i] = (des.list)[i];
		kP = new keyPoint(*des.kP);
	}

};

// Liste de descripteur
typedef  std::vector<descriptor*> listDescriptor;

// Creation de descriptors avec la liste des points cl� et de l'image int
descriptor* makeDescriptor(imageIntegrale* imgInt, keyPoint* pC);

// creation liste descriptor
listDescriptor* getDescriptor(imageIntegrale* imgInt, listKeyPoints* lPC);

// creation liste points cles
listDescriptor* getKeyPoints(image *img, listKeyPoints* lKP, float threshold);

#endif
#pragma once
