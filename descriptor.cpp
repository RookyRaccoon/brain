
#include "descriptor.h"



// Pour une liste de points clés, on retourne une liste de desciptor associés
//Pour la memoire on suprime ensuite (peut etre a voir) l'image car on en a plus besoin ?????

listDescriptor* getDescriptor(imageIntegrale* imgInt, listKeyPoints* lPC)
{
	listDescriptor* lD = new listDescriptor();
	// descriptor des points cles
	for (int i = 0; i<(int)lPC->size(); i++)
		lD->push_back(makeDescriptor(imgInt, (*lPC)[i]));
	//delete imgInt;/*MemCheck*/???????????????????????????????????????????????????????????????????????????????
	return lD;
}



// Cree un descitpor dans un domaine carré de 20*echelle
descriptor* makeDescriptor(imageIntegrale* imgInt, keyPoint* pC)
{
	IMAGE_NORMALE scale = pC->scale;
	descriptor* desc = new descriptor();
	// Divise en zone 4x4 l'espace autour du point d'intéret (keypoint)

	// Orientation
	IMAGE_NORMALE cosP = cos(pC->orientation);
	IMAGE_NORMALE sinP = sin(pC->orientation);
	IMAGE_NORMALE norm = 0, u, v, gauss, responseU, responseV, responseX, responseY;

	// Divise en 16 l'espace du keypoint
	for (int i = 0; i<DESCRIPTOR_TAILLE_1D; i++)
	{
		for (int j = 0; j<DESCRIPTOR_TAILLE_1D; j++)
		{
			(desc->list[DESCRIPTOR_TAILLE_1D*i + j]).sumDx = 0;
			(desc->list[DESCRIPTOR_TAILLE_1D*i + j]).sumAbsDx = 0;
			(desc->list[DESCRIPTOR_TAILLE_1D*i + j]).sumDy = 0;
			(desc->list[DESCRIPTOR_TAILLE_1D*i + j]).sumAbsDy = 0;
			// Chaque zone 4x4 est divisée en 5x5
			for (int k = 0; k<5; k++)
			{
				for (int l = 0; l<5; l++)
				{
					// Haar
					u = (pC->x + scale * (cosP*((i - 2) * 5 + k + 0.5) - sinP * ((j - 2) * 5 + l + 0.5)));
					v = (pC->y + scale * (sinP*((i - 2) * 5 + k + 0.5) + cosP * ((j - 2) * 5 + l + 0.5)));
					responseX = haarX(imgInt, u, v, arrondir(scale)); 
					responseY = haarY(imgInt, u, v, arrondir(scale));

					// Poids de gausse
					gauss = gaussien(((i - 2) * 5 + k + 0.5), ((j - 2) * 5 + l + 0.5), 3.3);

					// rotation axe
					//responseU = gauss*( -responseX*sinP + responseY*cosP);
					//responseV = gauss*(responseX*cosP + responseY*sinP);
					responseU = gauss * (responseX*cosP + responseY * sinP);
					responseV = gauss * (-responseX * sinP + responseY * cosP);

					// LES DESCRIPTORS
					(desc->list[DESCRIPTOR_TAILLE_1D*i + j]).sumDx += responseU;
					(desc->list[DESCRIPTOR_TAILLE_1D*i + j]).sumAbsDx += val_absolue(responseU);
					(desc->list[DESCRIPTOR_TAILLE_1D*i + j]).sumDy += responseV;
					(desc->list[DESCRIPTOR_TAILLE_1D*i + j]).sumAbsDy += val_absolue(responseV);

				}
			}
			// norme vecteurs
			norm += (desc->list[DESCRIPTOR_TAILLE_1D*i + j]).sumAbsDx*(desc->list[DESCRIPTOR_TAILLE_1D*i + j]).sumAbsDx + (desc->list[DESCRIPTOR_TAILLE_1D*i + j]).sumAbsDy*(desc->list[DESCRIPTOR_TAILLE_1D*i + j]).sumAbsDy + ((desc->list[DESCRIPTOR_TAILLE_1D*i + j]).sumDx*(desc->list[DESCRIPTOR_TAILLE_1D*i + j]).sumDx + (desc->list[DESCRIPTOR_TAILLE_1D*i + j]).sumDy*(desc->list[DESCRIPTOR_TAILLE_1D*i + j]).sumDy);

		}
	}
	// Normalisation des descriptors, pour amélierer et assurer qu'il y a bien une invariance (= pas de facteurs qui feront varier le resultat)
	//aux changements de contraste
	
	norm = sqrtf(norm);
	if (norm != 0)
		for (int i = 0; i<DESCRIPTOR_TAILLE_1D*DESCRIPTOR_TAILLE_1D; i++)
		{
			(desc->list[i]).sumDx /= norm;
			(desc->list[i]).sumAbsDx /= norm;
			(desc->list[i]).sumDy /= norm;
			(desc->list[i]).sumAbsDy /= norm;
		}
	desc->kP = new keyPoint(*pC);
	return desc;
}


