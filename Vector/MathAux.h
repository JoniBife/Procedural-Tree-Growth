#ifndef MATHAUX_H
#define MATHAUX_H

#include <iostream>
#include <math.h>

/*
*
* Checks if the floats A and B are equal
* using FLT_EPISILON as error margin
* 
* @param A first float
* @param B second float
* 
* @return true if A equals B else false 
*/
bool cmpf(float A, float B);

float round6(float number);

float randomFloat();

float degreesToRadians(float angle);

#endif