#pragma once
#ifndef INTEGRAL
#define INTEGRAL

#include "image.h"


// Convolution formule (4) dans un domaine T=[a,b]x[c,d]
//Box filter : BT= U(x-a,y-c)+U(x-b-1,y-d-1)-U(x-a,y-d-1)-U(x-b-1,y-c) ou U image integrale
/* */
//Convolution carrée
inline double squareConvolutionXY(imageIntegrale* imgInt, int a, int b, int c, int d, int x, int y)
{
	int a1 = x - a;
	int a2 = y - b;
	int b1 = a1 - c;
	int b2 = a2 - d;
	return ((*imgInt)(b1, b2) + (*imgInt)(a1, a2) - (*imgInt)(b1, a2) - (*imgInt)(a1, b2));// Note: Pas de L^2 normalisation ici
}



// Convolution pour [-1;+1]
inline IMAGE_INTEGRALE haarX(imageIntegrale* img, int x, int y, int lambda)
{

	return squareConvolutionXY(img, 0, -lambda - 1, lambda + 1, lambda * 2 + 1, x, y)
		- (squareConvolutionXY(img, 1, -lambda - 1, -lambda - 1, lambda * 2 + 1, x, y));


}

// Convolution pour [-1;+1]
inline IMAGE_INTEGRALE haarY(imageIntegrale* img, int x, int y, int lambda)
{
	return -(squareConvolutionXY(img, -lambda - 1, 1, 2 * lambda + 1, -lambda - 1, x, y) +
		squareConvolutionXY(img, -lambda - 1, 0, 2 * lambda + 1, lambda + 1, x, y));
}

#endif

