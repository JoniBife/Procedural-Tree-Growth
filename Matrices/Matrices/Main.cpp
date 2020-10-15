#include <iostream>
#include "Vec3.h"
#include <time.h>
#include "Mat2.h"

int main()
{
	//srand((unsigned int)time(NULL));

	//std::cout << "-------- WITHOUT ZERO -----------" << std::endl;

	Mat2 in1;
	Mat2 in2;
	std::cin >> in1 >> in2;

	std::cout << in1 + in2;
	std::cout << in1 - in2;
	std::cout << in1 * in2;

	/*for (int i = 0; i < 10; ++i) {
		Vec3 a = Vec3::random();
		Vec3 b = Vec3::random();
		Vec3 c = Vec3::random();

		std::cout << "a = " << a << std::endl;
		std::cout << "b = " << b << std::endl;
		std::cout << "c = " << c << std::endl;

		Vec3 left = cross(a, cross(b, c));
		Vec3 right = dot(a, c) * b - dot(a, b) * c;

		std::cout << "left = " << left << std::endl;
		std::cout << "right = " << right << std::endl;

		if (left == right) {
			std::cout << "Success!" << "\n" << std::endl;
		}
	}

	std::cout << "\n -------- WITH ZERO -----------" << std::endl;

	for (int i = 0; i < 10; ++i) {
		Vec3 a = Vec3::random();
		Vec3 b = Vec3::random();
		Vec3 c = Vec3::ZERO;

		std::cout << "a = " << a << std::endl;
		std::cout << "b = " << b << std::endl;
		std::cout << "c = " << c << std::endl;

		Vec3 left = cross(a, cross(b, c));
		Vec3 right = dot(a, c) * b - dot(a, b) * c;

		std::cout << "left = " << left << std::endl;
		std::cout << "right = " << right << std::endl;

		if (left == right) {
			std::cout << "Success!" << "\n" << std::endl;
		}
	}

	Vec3 a(1, 0, 0);
	Vec3 b(0, 1, 0);
	Vec3 c(0, 0, 1);

	std::cout << "a = " << a << std::endl;
	std::cout << "b = " << b << std::endl;
	std::cout << "c = " << c << std::endl;

	Vec3 left = cross(a, cross(b, c));
	Vec3 right = dot(a, c) * b - dot(a, b) * c;

	std::cout << "left = " << left << std::endl;
	std::cout << "right = " << right << std::endl;

	if (left == right) {
		std::cout << "Success!" << "\n" << std::endl;
	}*/
}



