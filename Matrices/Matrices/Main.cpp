#define _USE_MATH_DEFINES

#include <time.h>
#include <iostream>
#include <math.h>
#include "Vec3.h"
#include "Vec2.h"
#include "Mat2.h"
#include "Mat4.h"
#include "Mat3.h"
#include "MathAux.h"

// TODO Reduce the number of casts from integer to float (in situations like: 2 should be 2.0f)

Mat3 randomInvertible() {
	
	Mat3 invert;
	
	do {
	
		invert = Mat3::randomMatrix(-10, 10);

		if (invert.determinant() != 0) {
			break;
		}
	
	} while (true);

	return invert;
}

int main()
{
	//AVT CHALLENGE

	srand((unsigned int)time(NULL));


	// -----------------CHALLENGE 1-------------------------------------

	std::cout << "\n-------CHALLENGE 1-----\n";

	for (int i = 0; i < 10; i++) {

		Mat3 a = randomInvertible();
		Mat3 b = randomInvertible();

		std::cout << "\n-------MATRIX A-----\n" << a;
		std::cout << "\n-------MATRIX B-----\n" << b;

		Mat3 transposeL = (a * b).transpose();
		Mat3 transposeR = b.transpose() * a.transpose();

		std::cout << "\n-------(A*B)T-----\n" << transposeL;
		std::cout << "\n------- BT*AT-----\n" << transposeR;

		if (transposeR == transposeL) {

			std::cout << "\n TRANSPOSE EQUALITY: TRUE\n";

		}
		else
			std::cout << "\n TRANSPOSE EQUALITY: FALSE\n";
	}


	// -----------------CHALLENGE 2-------------------------------------

	std::cout << "\n-------CHALLENGE 2-----\n";

	Mat3 m = { 2, 3, 1,
			   3, 4, 1,
			   3, 7, 2 };

	Mat3 n = { 1, 0, 0,
			   0, 0, -2,
			   -5, 0, -9 };

	std::cout << "\n-------M_Transpose-----\n" << m.transpose();
	std::cout << "\n-------N_Transpose-----\n" << n.transpose();
	std::cout << "\n-------M_Determinant = " << m.determinant() << "\n";
	std::cout << "\n-------N_Determinant = " << n.determinant() << "\n";

	Mat3 mInverse;
	bool canInvert = m.inverse(mInverse);

	if (canInvert) {
		std::cout << "\n-------M_inverse-----\n" << mInverse;
	}
	else {
		std::cout << "\n Cannot invert matrix M \n";
	}

	Mat3 nInverse;
	canInvert = n.inverse(nInverse);

	if (canInvert) {
		std::cout << "\n-------N_inverse-----\n" << nInverse;
	}
	else {
		std::cout << "\n Cannot invert matrix N \n";
	}

	float mOpenGL[9];
	m.toOpenGLFormat(mOpenGL);

	float nOpenGL[9];
	n.toOpenGLFormat(nOpenGL);

	std::cout << "\n-------M_OpenGL-----\n";
	for (int i = 0; i < 9; ++i) {
		std::cout << mOpenGL[i] << " ";
	}
	std::cout << "\n-------N_OpenGL-----\n";
	for (int i = 0; i < 9; ++i) {
		std::cout << nOpenGL[i] << " ";
	}

	// -----------------CHALLENGE 3-------------------------------------

	std::cout << "\n\n-------CHALLENGE 3-----\n";

	float angleRad = (34 * M_PI) / 180;

	Mat4 translation = Mat4::translation(4, 5, 6);
	Mat4 scaling = Mat4::scaling(4, 5, 6);
	Mat4 rotation = Mat4::rotation(angleRad, Vec3::Z);

	Mat4 tResult = { 1, 0, 0, 4,
					0, 1, 0, 5,
					0, 0, 1, 6,
					0, 0, 0, 1 };

	Mat4 sResult = { 4, 0, 0, 0,
					0, 5, 0, 0,
					0, 0, 6, 0,
					0, 0, 0, 1 };

	Mat4 rResult = { cosf(angleRad), -sinf(angleRad), 0, 0,
					 sinf(angleRad), cosf(angleRad), 0, 0,
				  	 0, 0, 1, 0,
					 0, 0, 0, 1 };

	
	std::cout << "\n-------TRANSLATION-----\n" << translation;
	if (translation == tResult) {

		std::cout << "\n TRANSLATION GOOD\n";

	}
	else
		std::cout << "\n TRANSLATION BAD\n";
	std::cout << "\n---------SCALING-------\n" << scaling;
	if (scaling == sResult) {

		std::cout << "\n SCALING GOOD\n";

	}
	else
		std::cout << "\n SCALING BAD\n";
	std::cout << "\n---------ROTATION------\n" << rotation;
	if (rotation == rResult) {

		std::cout << "\n ROTATION GOOD\n";

	}
	else
		std::cout << "\n ROTATION BAD\n";
	
}



