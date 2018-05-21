

#include "matchlib.h"




std::vector<Match> matchDescriptor(listDescriptor * l1, listDescriptor * l2)
{
	// Match utilisent un ratio entre les descriptors de l1 et les deux plus proches de l2
	float thrm = RATIO * RATIO;

	std::vector<Match> matches;

	// Matching non sym
	for (int i = 0; i<(int)l1->size(); i++)
	{
		int position = -1;
		float d1 = 3;
		float d2 = 3;

		for (int j = 0; j<(int)l2->size(); j++)
		{
			float d = euclideanDistance((*l1)[i], (*l2)[j]);
			// On seclectionne les descripteurs les plus proches
			if ((((*l1)[i])->kP)->signLaplacian == (((*l2)[j])->kP)->signLaplacian)
			{
				d2 = ((d2>d) ? d : d2);
				if (d1>d)
				{
					position = j;
					d2 = d1;
					d1 = d;
				}
			}
		}

		// on les match (enfin on essaye)
		if (position >= 0 && thrm*d2>d1)
		{
			Match match;
			match.x1 = ((*l1)[i]->kP)->x;
			match.y1 = ((*l1)[i]->kP)->y;
			match.x2 = ((*l2)[position]->kP)->x;
			match.y2 = ((*l2)[position]->kP)->y;
			matches.push_back(match);
		}
	}
	return matches;
}



// distance euclidienne entre les deux descriptors
float euclideanDistance(descriptor* a, descriptor* b)
{
	float sum = 0;
	for (int i = 0; i<16; i++)
	{
		sum += ((a->list)[i].sumDx - (b->list)[i].sumDx)*((a->list)[i].sumDx - (b->list)[i].sumDx)
			+ ((a->list)[i].sumDy - (b->list)[i].sumDy)*((a->list)[i].sumDy - (b->list)[i].sumDy)
			+ ((a->list)[i].sumAbsDy - (b->list)[i].sumAbsDy)*((a->list)[i].sumAbsDy - (b->list)[i].sumAbsDy)
			+ ((a->list)[i].sumAbsDx - (b->list)[i].sumAbsDx)*((a->list)[i].sumAbsDx - (b->list)[i].sumAbsDx);
	}
	return sum;
}


// On clean
void cleanMatch(std::vector<Match>* list)
{

	bool* toRemove = new bool[list->size()];
	memset(toRemove, 0, sizeof(bool)*list->size());
	for (int i = 0; i<(int)list->size(); i++)
	{

		int x = (*list)[i].x1;
		int y = (*list)[i].y1;
		if (!toRemove[i])
			for (int j = i + 1; j<(int)list->size(); j++)
			{
				int x_ = (*list)[i].x2;
				int y_ = (*list)[i].y2;

				if (x_ == x && y == y_)
				{
					toRemove[i] = true;
					toRemove[j] = true;
				}
			}
	}

	for (int i = (int)list->size() - 1; i >= 0; i--)
		if (toRemove[i])
		{
			list->erase(list->begin() + i);
		}
	delete[] toRemove;
}


std::vector<Match> cleanMatchORSA(std::vector<Match> match_coor)
{
	// On utilise ORSA pour filtrer les matchs incorrects
	// On enregistre les coordonnées des matchs ORSA  10 points min (code ASIFT).

	std::vector<float> index;
	// Guoshen Yu, 2010.09.23
	// index.clear();

	int t_value_orsa = 10000;
	int verb_value_orsa = 0;
	int n_flag_value_orsa = 0;
	int mode_value_orsa = 2;
	int stop_value_orsa = 0;

	std::vector<Match>  finalMatches;
	float largeur = 0;
	float hauteur = 0;
	for (int i = 0; i<(int)match_coor.size(); i++)
	{

		// taille des images
		largeur = (match_coor[i].x1>largeur) ? match_coor[i].x1 : largeur;
		largeur = (match_coor[i].x2>largeur) ? match_coor[i].x2 : largeur;
		hauteur = (match_coor[i].y1>hauteur) ? match_coor[i].y1 : hauteur;
		hauteur = (match_coor[i].y2>hauteur) ? match_coor[i].y2 : hauteur;
	}

	// filtre epipolaire avec l'algo Moisan-Stival ORSA
	orsa(largeur, hauteur, match_coor, index, t_value_orsa, verb_value_orsa, n_flag_value_orsa, mode_value_orsa, stop_value_orsa);

	for (int cc = 0; cc < (int)index.size(); cc++)
		finalMatches.push_back(match_coor[index[cc]]);

	return finalMatches; //youhou !!!
}



