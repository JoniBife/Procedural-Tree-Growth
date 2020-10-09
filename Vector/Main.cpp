#include <iostream>
#include "Vec3.h"

int main()
{
	// Rodrigues rotation formula

	Vec3 v(1.5f, 3.2f, 0.8f);
	float theta = 24;
	Vec3 k(8, 2, 0);

	Vec3 vrot = rodrigues(v, theta, k);
	std::cout << "vrot = " << vrot << std::endl;

	Vec3 v2(0.5f, 1.0f, 1.0f);
	float theta2 = 0;
	Vec3 k2(8, 2, 0);

	Vec3 vrot2 = rodrigues(v2, theta2, k2);
	std::cout << "vrot2 = " << vrot2 << std::endl;


	// Equalities and inequalities

	Vec3 a(1, 2, 3);
	Vec3 b(0.1f, 0.2f, 0.3f);
	Vec3 c(-0.9f, -1.79f, -2.7f);
	Vec3 d(1.1f, 2.2f, 3.3f);

	if (a == (b * 10)) {
		std::cout << "Success" << std::endl;
	}

	if ((a + b) == d) {
		std::cout << "Success" << std::endl;
	}

	if ((a - b) != c) {
		std::cout << "Success" << std::endl;
	}
}



