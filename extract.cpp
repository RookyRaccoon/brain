#include "extract.h"
using namespace std;

// Doit etre exec commme surf file1.png descriptor.txt
int main(int argc, char** argv)
{

	if (argc>4)
	{
		cerr << "Erreur." << endl;
		return EXIT_FAILURE;
	}
	if (argc<3)
	{
		cerr << "Erreur" << endl;
		return EXIT_FAILURE;
	}

	// seuil hessienne
	float threshold = 1000;

	if (argc == 4)
		threshold = atoi(argv[3]);; // Peut etre modifié pour la detection du hessien

									// input de l'image
	string imgName = argv[1];
	// output descriptors.txt
	string txtFile = argv[2];

	size_t nx, ny;
	unsigned char* data;





	if (NULL == (data = read_png_u8_gray(imgName.c_str(), &nx, &ny)) || nx<2 || ny<2)
	{
		cerr << "Erreur: image non lue, veuillez utiliser une image d'au moins 2x2 PNG et relancer le programme." << endl;
		return EXIT_FAILURE;
	}

	//Input image dans le format SURF IPOL
	image* img = new image((int)nx, (int)ny, data);

	free(data);

	// Normalisation des avec valeurs 0 256
	img->normalizeImage();


	// liste pc
	listKeyPoints* l = new listKeyPoints();
	listDescriptor* listDesc;

	// pc detection et description
	listDesc = getKeyPoints(img, l, threshold);


	//resultats ecrits dans fichier txt
	std::ofstream f(txtFile.c_str(), std::ios::trunc);

	if (!f)
	{
		std::cerr << "Erreur on ne peut pas faire de fichier texte." << std::endl;
		exit(1);
	}

	f << 64 << "\n";
	f << listDesc->size() << "\n";
	for (int i = 0; i<(int)listDesc->size(); i++)
	{
		f << ((*listDesc)[i]->kP)->x << " " << ((*listDesc)[i]->kP)->y << " " << ((*listDesc)[i]->kP)->scale << " " << ((*listDesc)[i]->kP)->orientation << " ";
		for (int j = 0; j<16; j++)
			f << (((*listDesc)[i]->list)[j]).sumDx << " " << (((*listDesc)[i]->list)[j]).sumDy << " " << (((*listDesc)[i]->list)[j]).sumAbsDx << " " << (((*listDesc)[i]->list)[j]).sumAbsDy << " ";
		f << ((((*listDesc)[i]->kP)->signLaplacian) ? 1 : -1);
		f << "\n";
	}
	f.close();



	// Free memory
	/*MemCheck*/
	for (int i = 0; i<(int)l->size(); i++)
		delete((*l)[i]);
	for (int i = 0; i<(int)listDesc->size(); i++)
		delete((*listDesc)[i]);
	delete l;
	delete listDesc;
	delete img;

	return (EXIT_SUCCESS);
}
