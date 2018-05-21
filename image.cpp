#include"image.h"

//Constructeurs
image::image(int x, int y, unsigned char* data) :largeur(x), hauteur(y) {
	//alloc:
	img = new IMAGE_NORMALE[largeur*hauteur];
	for (int i = 0; i<x; i++) {
		for (int j = 0; j<y; j++) {
			img[largeur*j + i] = (int)data[largeur*j + i];
		}
	}
}

image::image(int x, int y) :largeur(x), hauteur(y) {
	img = new IMAGE_NORMALE[largeur*hauteur];
	memset(img, 0, largeur*hauteur * sizeof(IMAGE_NORMALE)); //attribue taille fixe an block memoire pointé par img
}

image::image(image *im) : hauteur(im->hauteur), largeur(im->largeur) {

	img = new IMAGE_NORMALE[largeur*hauteur];
	memset(img, 0, largeur*hauteur * sizeof(IMAGE_NORMALE));
}

// Afficher 3 images avec leurs matchs avec ORSA.
void image::printImagePara(char fileName[], image* para) {

	float* data = new float[3 * largeur*hauteur];
	float max, min;
	max = img[0];
	min = img[0];

	for (int i = 0; i<largeur*hauteur; i++)
	{
		if (min>img[i]) min = img[i];
		if (max<img[i]) max = img[i];
	}

	for (int i = 0; i<largeur*hauteur; i++)
	{
		float lambda = (img[i] - max) / (min - max);
		data[i] = arrondir(0 * lambda + (1 - lambda) * 255);
		data[largeur*hauteur + i] = arrondir(0 * lambda + (1 - lambda) * 255);
		data[2 * largeur*hauteur + i] = arrondir(0 * lambda + (1 - lambda) * 255);
		if ((*para)(i%para->get_largeur(), i / para->get_largeur()) != 0)
		{
			data[2 * largeur*hauteur + i] = 0;
			data[0 * largeur*hauteur + i] = arrondir(0 * lambda + (1 - lambda) * 130) + (*para)(i%para->get_largeur(), i / para->get_largeur());
			data[1 * largeur*hauteur + i] = 0;
		}
	}

	write_png_f32(fileName, data, largeur, hauteur, 3);
	delete[] data;

}

// On se place dans l'intervalle [0;255] pour la normalisation de l'image
void image::normalizeImage() {

	IMAGE_NORMALE min = img[0], max = img[0];

	for (int i = 0; i<largeur; i++)
		for (int j = 0; j<hauteur; j++)
		{
			min = (img[j*largeur + i]<min) ? img[j*largeur + i] : min;
			max = (img[j*largeur + i]>max) ? img[j*largeur + i] : max;
		}

	for (int i = 0; i<largeur; i++)
		for (int j = 0; j<hauteur; j++)
			img[j*largeur + i] = (int)255 * ((img[j*largeur + i] - min) / (max - min));
}



// Taille de l'image échantillonnée
void image::getSampledImage(int& l, int& h, int sample) {

	l = largeur/ sample;
	h = hauteur / sample;

}

// Marge de l'image + allocation de l'image
image* image::marImage(int marge) {
	image* img2 = new image(largeur + 2 * marge, hauteur + 2 * marge);
	int i0, j0;

	for (int i = -marge; i<largeur + marge; i++)
		for (int j = -marge; j<hauteur + marge; j++)
		{
			i0 = i;
			j0 = j;
			if (i0<0)
				i0 = -i0;
			if (j0<0)
				j0 = -j0;
			i0 = i0 % (2 * largeur);
			j0 = j0 % (2 * hauteur);
			if (i0 >= largeur)
				i0 = 2 * largeur - i0 - 1;
			if (j0 >= hauteur)
				j0 = 2 * hauteur - j0 - 1;

			(*img2)(i + marge, j + marge) = (*this)(i0, j0);
		}

	return img2;

}



imageIntegrale::imageIntegrale(image* im) {

	marge = 312;// size descripteur * max size L = 4*0.4*195;
	largeur = im->get_largeur() + 2 * marge;
	hauteur = im->get_hauteur() + 2 * marge;

	image* img_mar = im->marImage(marge); // Pad the image

	img = new IMAGE_INTEGRALE[largeur*hauteur];
	computeImageIntegrale(img_mar);
	delete img_mar;

}

//destructeur
imageIntegrale::~imageIntegrale() {
	delete[] img;
}


// Computation de l'image intégrale
void imageIntegrale::computeImageIntegrale(image* image) {

	// Initialization
	img[0] = (*image)(0, 0);

	// premier rang
	for (int i = 1; i<image->get_largeur(); i++)
		img[i] = img[i - 1] + (*image)(i, 0);

	// Recursion
	for (int j = 1; j<image->get_hauteur(); j++)
	{
		IMAGE_INTEGRALE h = 0;
		for (int i = 0; i<image->get_largeur(); i++)
		{
			h += (*image)(i, j);
			img[i + largeur * j] = img[i + largeur * (j - 1)] + h;
		}
	}

}

