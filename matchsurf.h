#ifndef MATCH
#define MATCH


#include "descriptor.h"

#include <fstream>
#include <sstream>
#include <stdlib.h>
using namespace std;


// on retourne un descripor en utilisant le fichier txt extrait par extract
listDescriptor* getDescriptorFromRawFile(string nameFile);

listDescriptor* getDescriptorFromRawFile(string nameFile)
{
	// lecture chaque ligne
	ifstream infile(nameFile.c_str());
	listDescriptor* listDesc = new listDescriptor();
	int k = 0;
	while (infile)
	{

		// deux premieres non utiles
		if (k<2)
		{
			k++;
			continue;
		}
		string line;
		if (!getline(infile, line)) break;
		istringstream ss(line);

		int i = 0;
		keyPoint* kp = new keyPoint();
		descriptor* desc = new descriptor();
		desc->kP = kp;

		while (ss)
		{

			// x et y echelle, orientation d1... 64; signe laplacien
			string word;
			if (!getline(ss, word, ' ')) break;
			if (i == 0)
				kp->x = strtod(word.c_str(), NULL);
			else if (i == 1)
				kp->y = strtod(word.c_str(), NULL);
			else if (i == 2)
				kp->scale = strtod(word.c_str(), NULL);
			else if (i == 3)
				kp->orientation = strtod(word.c_str(), NULL);
			else if (i == 68)
				kp->signLaplacian = strtod(word.c_str(), NULL);
			else
			{
				int j = (i - 4) / 4;
				int k = (i - 4) % 4;
				switch (k)
				{
				case 0:
					((desc->list)[j]).sumDx = strtod(word.c_str(), NULL);
					break;
				case 1:
					((desc->list)[j]).sumDy = strtod(word.c_str(), NULL);
					break;
				case 2:
					((desc->list)[j]).sumAbsDx = strtod(word.c_str(), NULL);
					break;

				case 3:
					((desc->list)[j]).sumAbsDy = strtod(word.c_str(), NULL);
					break;
				}
			}

			i++;
		}

		listDesc->push_back(desc);
	}
	return listDesc;
}

#endif#pragma once
