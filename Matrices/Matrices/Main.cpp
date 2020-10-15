#include <iostream>
#include "Vec3.h"
#include "Vec2.h"
#include <time.h>
#include "Mat2.h"
#include "Mat4.h"

// TODO Reduce the number of casts from integer to float (in situations like: 2 should be 2.0f)

int main()
{
	//srand((unsigned int)time(NULL));

	//std::cout << "-------- WITHOUT ZERO -----------" << std::endl;

	Mat2 in1;
	Mat2 in2;
	std::cout << "insert matrix 2x2 A:" << std::endl;
	std::cin >> in1;
	std::cout << "insert matrix 2x2 B:" << std::endl;
	std::cin >> in2;

	std::cout << "\n-------SUM------\n"<< in1 + in2;
	std::cout << "\n-------SUB------\n" << in1 - in2;
	std::cout << "\n-------MUL------\n" << in1 * in2;
	std::cout << "\n-------TRANS------\n" << in1.transpose();
	std::cout << "\n-------EQL------\n" << ((in1 == in2) ? "true" : "false");
	std::cout << "\n-------NOT_EQL------\n" << ((in1 != in2) ? "true" : "false");
	std::cout << "\n-------MUL_S------\n";
	std::cout << "insert scalar: ";
	float s;
	std::cin >> s;
	std::cout << in1 * s;
	std::cout << "\n-------MUL_V-----\n" << in1*Vec2(1,1);
	std::cout << "\n-------DET-----\n" << in1.determinant();
	std::cout << "\n-------INV-----\n";
	Mat2 inv;
	bool canInvert = in1.inverse(inv);
	if (canInvert) {
		std::cout << inv;
	}
	else {
		std::cout << "Can't invert matrix A";
	}

	std::cout << std::endl;
	

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



