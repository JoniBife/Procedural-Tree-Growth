#ifndef BOUNDING_SPHERE_H
#define BOUNDING_SPHERE_H

#include "../math/MathAux.h"
#include "../math/Vec3.h"

/*
* This struct simply represents a Spherical Bounding Volume
* that is used to estimate the available space for module 
* growth.
*/
struct BoundingSphere {

	Vec3 center;
	float radius;

	BoundingSphere();

	BoundingSphere(const Vec3& center, float radius);

	float volume();

	/*
	* Calculates the volume of the sphere-sphere intersection using
	* the sum of the volume of the two spherical caps that result from
	* the intersection.
	* 
	* Reference:
	* https://mathworld.wolfram.com/Sphere-SphereIntersection.html
	*/
	float intersectVolume(BoundingSphere& boundingSphere);
};



#endif
