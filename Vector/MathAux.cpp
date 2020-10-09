#include "MathAux.h"

#define EPSILON 0.00005f

bool cmpf(float A, float B)
{
    return fabsf(A - B) < EPSILON;
}

float round6(float number) {
    return roundf(number * pow(10, 6)) / pow(10, 6);
}

float randomFloat() {
    float a = 5.0;
    return (float(rand()) / float((RAND_MAX))) * a;
}