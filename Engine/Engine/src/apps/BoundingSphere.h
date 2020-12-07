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

	BoundingSphere(const Vec3& center, float radius) : center(center), radius(radius) { }

	float volume() {
		return (4.0f / 3.0f) * PI * (radius * radius * radius);
	}

	/*
	* Calculates the volume of the sphere-sphere intersection using
	* the sum of the volume of the two spherical caps that result from
	* the intersection.
	* 
	* Reference:
	* https://mathworld.wolfram.com/Sphere-SphereIntersection.html
	*/
	float intersectVolume(BoundingSphere& boundingSphere) {

		// Distance from centers
		float d = (center - boundingSphere.center).magnitude();

		// Assigning each radius to a variable so that the expression is more easily compared to the original in wolfram
		float R = this->radius;
		float r = boundingSphere.radius;

		// If the distance from centers is larger or equal than the sum of their radius then they don't intersect
		float radiusSum = R + r;
		if (cmpf(d, radiusSum) || d > radiusSum) {
			return 0.0f;
		}

		// When the distance is 0 we compare the volumes
		if (cmpf(0, d)) {
			float volumeA = this->volume();
			float volumeB = boundingSphere.volume();

			if (!cmpf(volumeA, volumeB) && volumeA < volumeB)
				return volumeA;

			return volumeB;
		}	

		// Special case for when both radius are equal, the expression can be simplified
		if (cmpf(R,r)) {
			return (1.0f / 12.0f) * PI * (4.0f * R + d) * SQR(2 * R - d);
		}

		return (PI * SQR(R + r - d) * (SQR(d) + 2 * d * r - 3 * SQR(r) + 2 * d * R + 6 * r * R - 3 * SQR(R))) / (12 * d);
	}
};



#endif
