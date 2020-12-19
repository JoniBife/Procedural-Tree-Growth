#include "BoundingSphere.h"

BoundingSphere::BoundingSphere() : center({ 0.0f, 0.0f, 0.0f }), radius(0.0f) { }

BoundingSphere::BoundingSphere(const Vec3& center, float radius) : center(center), radius(radius) { }

float BoundingSphere::volume() {
	return (4.0f / 3.0f) * PI * (radius * radius * radius);
}

float BoundingSphere::intersectVolume(BoundingSphere& boundingSphere) {

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

	float smallerRadius = R;
	float largerRadius = r;

	if (R > r) {
		smallerRadius = r;
		largerRadius = R;
	}

	// Checking if one of the spheres is within the other, if thats the case we return the one with the least volume
	if (cmpf(0.0f, d) || smallerRadius + d < largerRadius) {
		return (4.0f / 3.0f) * PI * (smallerRadius * smallerRadius * smallerRadius);
	}

	// Special case for when both radius are equal, the expression can be simplified
	if (cmpf(R, r)) {
		return (1.0f / 12.0f) * PI * (4.0f * R + d) * SQR(2 * R - d);
	}

	return (PI * SQR(R + r - d) * (SQR(d) + 2 * d * r - 3 * SQR(r) + 2 * d * R + 6 * r * R - 3 * SQR(R))) / (12 * d);
}