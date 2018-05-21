#ifndef LIB_MATCH_SURF
#define LIB_MATCH_SURF
#include "descriptor.h"
#include "C:\Users\edeha\Documents\ECOLE\s4\Brainmatch\BrainMatch\fichiers_externes/libMatch/match.h"
#include "C:\Users\edeha\Documents\ECOLE\s4\Brainmatch\BrainMatch\fichiers_externes/orsa.h"
#include "stdafx.h"
#include <iostream>
// Match deux descrip
std::vector<Match>  matchDescriptor(listDescriptor * l1, listDescriptor * l2);

// Netoiyage de deux matchs repetés
void cleanMatch(std::vector<Match>* list);

// distance euclidienne entre deux descri
float euclideanDistance(descriptor *a, descriptor* b);

// nettoie match avec ORSA
std::vector<Match> cleanMatchORSA(std::vector<Match> match_coor);
#endif
#pragma once
