#include "keypoint.h"
#include <iostream>
#include <fstream>

// Liste points d'interet et descript pour un seuil donnée (threshold)
listDescriptor* getKeyPoints(image *img, listKeyPoints* lKP, float threshold)
{

	// Image integrale
	imageIntegrale* imgInt = new imageIntegrale(img);

	// Pointeur sur les hessiens et signe laplacien
	image*  hessian[LONGMAX];
	image* signLaplacian[LONGMAX];

	// Autres variables
	double Dxx, Dxy, Dyy;
	int intervalCounter, octaveCounter, x, y, w, h, xcoo, ycoo, lp1, l3, mlp1p2, lp1d2, l2p1, pow2, sample, l;
	double nxy, nxx;

	// boucle pour octave
	for (octaveCounter = 0; octaveCounter<OCTAVE; octaveCounter++)
	{
		pow2 = pow(2, octaveCounter + 1);

		sample = pow(SAMPLE_IMAGE, octaveCounter);// echantillonnage

		img->getSampledImage(w, h, sample);// cree echnatillon avec 0

										   // init mem
		for (int i = 0; i<LONGMAX; i++)
		{
			hessian[i] = new image(w, h);
			signLaplacian[i] = new image(w, h);
		}

		// boucles pour longmax
		for (intervalCounter = 0; intervalCounter<LONGMAX; intervalCounter++)
		{
			l = pow2 * (intervalCounter + 1) + 1; // echelle

												  // gamma, predef pour plus vite
			lp1 = -l + 1;
			l3 = 3 * l;
			lp1d2 = (-l + 1) / 2;
			mlp1p2 = (-l + 1) / 2 - l;
			l2p1 = 2 * l - 1;

			nxx = sqrt(6 * l*(2 * l - 1));// filtres xx et yy
			nxy = sqrt(4 * l*l);// xy


								// hessien de chaque point (long)
			for (y = 0; y<h; y++)
			{
				for (x = 0; x<w; x++)
				{
					// echantillonage
					xcoo = x * sample;
					ycoo = y * sample;

					// FILTRE SECOND ORDRE
					Dxx = squareConvolutionXY(imgInt, lp1, mlp1p2, l2p1, l3, xcoo, ycoo) - 3 * squareConvolutionXY(imgInt, lp1, lp1d2, l2p1, l, xcoo, ycoo);
					Dxx /= nxx;

					Dyy = squareConvolutionXY(imgInt, mlp1p2, lp1, l3, l2p1, xcoo, ycoo) - 3 * squareConvolutionXY(imgInt, lp1d2, lp1, l, l2p1, xcoo, ycoo);
					Dyy /= nxx;
					Dxy = squareConvolutionXY(imgInt, 1, 1, l, l, xcoo, ycoo) + squareConvolutionXY(imgInt, 0, 0, -l, -l, xcoo, ycoo)
						+ squareConvolutionXY(imgInt, 1, 0, l, -l, xcoo, ycoo) + squareConvolutionXY(imgInt, 0, 1, -l, l, xcoo, ycoo);

					Dxy /= nxy;

					// hessien et laplacien
					(*hessian[intervalCounter])(x, y) = (Dxx*Dyy - 0.8317*(Dxy*Dxy));
					(*signLaplacian[intervalCounter])(x, y) = Dxx + Dyy>0;
				}
			}



		}

		IMAGE_NORMALE x_, y_, s_;

		// detection pt cle
		for (intervalCounter = 1; intervalCounter<LONGMAX - 1; intervalCounter++)
		{
			l = (pow2*(intervalCounter + 1) + 1);
			// points bordure
			for (int y = 1; y<h - 1; y++)
				for (int x = 1; x<w - 1; x++)
					if (isMaximum(hessian, x, y, intervalCounter, threshold))
					{
						x_ = x * sample;
						y_ = y * sample;
						s_ = 0.4*(pow2*(intervalCounter + 1) + 2); // box size or scale
																   // Affine refinement is performed for a given octave and sampling
						if (interpolationScaleSpace(hessian, x, y, intervalCounter, x_, y_, s_, sample, pow2))
							addKeyPoint(imgInt, x_, y_, (*(signLaplacian[intervalCounter]))(x, y), s_, lKP);
					}
		}

		/* MemCheck*/
		for (int j = 0; j<LONGMAX; j++)
		{
			delete hessian[j];
			delete signLaplacian[j];
		}

	}

	// Compute the descriptors
	return getDescriptor(imgInt, lKP);
}


// Creation point clé et ajout a la liste (vector)
void addKeyPoint(imageIntegrale* img, IMAGE_NORMALE i, IMAGE_NORMALE j, bool signL, IMAGE_NORMALE scale, listKeyPoints* lKP)
{
	keyPoint* pt = new keyPoint(i, j, scale, getOrientation(img, i, j, NOMBRE_SOUS_REGIONS, scale), signL);
	lKP->push_back(pt); //on ajoute a la fin de la liste
}


// Orientation du point clé
float getOrientation(imageIntegrale* imgInt, int x, int y, int sectors, IMAGE_NORMALE scale)
{
	sectors = NOMBRE_SOUS_REGIONS;
	IMAGE_NORMALE haarResponseX[sectors];
	IMAGE_NORMALE haarResponseY[sectors];
	IMAGE_NORMALE haarResponseSectorX[sectors];
	IMAGE_NORMALE haarResponseSectorY[sectors];
	IMAGE_NORMALE answerX, answerY;
	IMAGE_NORMALE gauss;

	int theta;

	memset(haarResponseSectorX, 0, sizeof(IMAGE_NORMALE)*sectors);
	memset(haarResponseSectorY, 0, sizeof(IMAGE_NORMALE)*sectors);
	memset(haarResponseX, 0, sizeof(IMAGE_NORMALE)*sectors);
	memset(haarResponseY, 0, sizeof(IMAGE_NORMALE)*sectors);

	// Contribution de tous les secteurs.
	for (int i = -6; i <= 6; i++)
		for (int j = -6; j <= 6; j++)
			if (i*i + j * j <= 36)
			{

				answerX = haarX(imgInt, x + scale * i, y + scale * j, arrondir(2 * scale));
				answerY = haarY(imgInt, x + scale * i, y + scale * j, arrondir(2 * scale));

				// Angles associés
				theta = (int)(atan2(answerY, answerX)* sectors / (2 * pi));
				theta = ((theta >= 0) ? (theta) : (theta + sectors));

				// poids de gauss
				gauss = gaussien(i, j, 2);

				// Cumulé
				haarResponseSectorX[theta] += answerX * gauss;
				haarResponseSectorY[theta] += answerY * gauss;
			}

	// reponse
	for (int i = 0; i<sectors; i++)
		for (int j = -sectors / 12; j <= sectors / 12; j++)
			if (0 <= i + j && i + j<sectors)
			{
				haarResponseX[i] += haarResponseSectorX[i + j];
				haarResponseY[i] += haarResponseSectorY[i + j];
			}
			else if (i + j < 0)
			{
				haarResponseX[i] += haarResponseSectorX[sectors + i + j];
				haarResponseY[i] += haarResponseSectorY[i + j + sectors];
			}

			else
			{
				haarResponseX[i] += haarResponseSectorX[i + j - sectors];
				haarResponseY[i] += haarResponseSectorY[i + j - sectors];
			}



	// Reponse max
	IMAGE_NORMALE max = haarResponseX[0] * haarResponseX[0] + haarResponseY[0] * haarResponseY[0];

	int t = 0;
	for (int i = 1; i<sectors; i++)
	{
		IMAGE_NORMALE norm = haarResponseX[i] * haarResponseX[i] + haarResponseY[i] * haarResponseY[i];
		t = ((max<norm) ? i : t);
		max = ((max<norm) ? norm : max);
	}


	// angle 
	return atan2(haarResponseY[t], haarResponseX[t]);
}



bool interpolationScaleSpace(image** img, int x, int y, int i, IMAGE_NORMALE &x_, IMAGE_NORMALE &y_, IMAGE_NORMALE &s_, int sample, int octaveValue)
{
	//si en dehors
	if (x <= 0 || y <= 0 || x >= img[i]->get_largeur() - 2 || y >= img[i]->get_hauteur() - 2)
		return false;
	IMAGE_NORMALE mx, my, mi, dx, dy, di, dxx, dyy, dii, dxy, dxi, dyi;

	//Nabla X (1D)
	dx = ((*(img[i]))(x + 1, y) - (*(img[i]))(x - 1, y)) / 2;
	dy = ((*(img[i]))(x, y + 1) - (*(img[i]))(x, y - 1)) / 2;
	di = ((*(img[i]))(x, y) - (*(img[i]))(x, y)) / 2;

	//Hessien X
	IMAGE_NORMALE a = (*(img[i]))(x, y);
	dxx = (*(img[i]))(x + 1, y) + (*(img[i]))(x - 1, y) - 2 * a;
	dyy = (*(img[i]))(x, y + 1) + (*(img[i]))(x, y + 1) - 2 * a;
	dii = ((*(img[i - 1]))(x, y) + (*(img[i + 1]))(x, y) - 2 * a);

	dxy = ((*(img[i]))(x + 1, y + 1) - (*(img[i]))(x + 1, y - 1) - (*(img[i]))(x - 1, y + 1) + (*(img[i]))(x - 1, y - 1)) / 4;
	dxi = ((*(img[i + 1]))(x + 1, y) - (*(img[i + 1]))(x - 1, y) - (*(img[i - 1]))(x + 1, y) + (*(img[i - 1]))(x - 1, y)) / 4;
	dyi = ((*(img[i + 1]))(x, y + 1) - (*(img[i + 1]))(x, y - 1) - (*(img[i - 1]))(x, y + 1) + (*(img[i - 1]))(x, y - 1)) / 4;

	// Det
	IMAGE_NORMALE det = dxx * dyy*dii - dxx * dyi*dyi - dyy * dxi*dxi + 2 * dxi*dyi*dxy - dii * dxy*dxy;

	if (det != 0) //matrice inversible
	{
		mx = -1 / det * (dx*(dyy*dii - dyi * dyi) + dy * (dxi*dyi - dii * dxy) + di * (dxy*dyi - dyy * dxi));
		my = -1 / det * (dx*(dxi*dyi - dii * dxy) + dy * (dxx*dii - dxi * dxi) + di * (dxy*dxi - dxx * dyi));
		mi = -1 / det * (dx*(dxy*dyi - dyy * dxi) + dy * (dxy*dxi - dxx * dyi) + di * (dxx*dyy - dxy * dxy));

		// si point stable"
		if (val_absolue(mx)<1 && val_absolue(my)<1 && val_absolue(mi)<1)
		{

			x_ = sample * (x + mx) + 0.5;// Center the pixels value
			y_ = sample * (y + my) + 0.5;
			s_ = 0.4*(1 + octaveValue * (i + mi + 1));
			return true;
		}

	}
	return false;
}

