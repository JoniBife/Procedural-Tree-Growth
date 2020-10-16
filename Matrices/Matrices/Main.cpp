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


	// 1

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


	//2

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

	//3

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
	
	
	//CGJ CHALLENGE

	/*Vec4 v(0.5f, 0, 0, 1);
	Vec3 n(0, 1, 0);

	Mat4 scale2 = Mat4::scaling(2);
	Mat4 rotate = Mat4::rotation(M_PI_2, Vec3::Y);
	Mat4 translate = Mat4::translation(1, 0, 0);

	std::cout << "\n-------VECTOR-----\n" << v;
	std::cout << "\n-------NORMAL-----\n" << n;

	std::cout << "\n-------SCALE------\n" << scale2;
	std::cout << "\n-------ROTATE------\n" << rotate;
	std::cout << "\n-------TRNSLT------\n" << translate;

	Mat4 model = translate * rotate * scale2;

	std::cout << "\n------MODEL---------\n" << model;

	Vec4 modelV = model * v;

	std::cout << "\n------MODEL*V-----------\n" << modelV;

	Mat3 normalM = model.toMat3();

	Mat3 normal;

	bool invertable = normalM.inverse(normal);

	normal = normal.transpose();

	std::cout << "\n\n--------NORMAL M--------\n" << normal;
	std::cout << "\n--------NORMAL*N--------\n" << normal * n;

	float opengl[16];
	model.toOpenGLFormat(opengl);

	for (int i = 0; i < 16; ++i) {
		std::cout << opengl[i] << " ";
	}

	scale2[0][0] = 3;

	std::cout << "\n--------[][] Test--------\n" << scale2;

	std::cout << std::endl;

	*/

	/*
	//TESTS
	Mat4 in1;
	Mat4 in2;
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
	std::cout << "\n-------MUL_V-----\n" << in1*Vec4(1,2,3,4);
	
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
	std::cout << "\n-------OPENGL_FORM-----\n";
	float opengl[16];
	in1.toOpenGLFormat(opengl);

	for (int i = 0; i < 16; ++i) {
		std::cout << opengl[i] << " ";
	}
	
	std::cout << "\n-------DUAL-----\n" << Mat3::dual(Vec3(1, 2, 3));
	
	std::cout << "\n-------SCA_S------\n" << Mat4::scaling(3);
	std::cout << "\n-------SCA------\n" << Mat4::scaling(1, 2, 3);
	std::cout << "\n-------TRNSLT------\n" << Mat4::translation(1, 2 , 3);
	std::cout << "\n-------ROT------\n" << Mat4::rotation(M_PI_2, Vec3::X);
	*/

	
}



