#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include "../math/Mat4.h"
#include "../math/Vec3.h"

Mat4 lookAt(Vec3 eye, Vec3 center, Vec3 up);

Mat4 ortho(float left, float right, float bottom, float top, float near, float far);

Mat4 perspective(float fovy, float aspectRatio, float near, float far);

#endif