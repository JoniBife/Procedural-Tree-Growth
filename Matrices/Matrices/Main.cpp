#define _USE_MATH_DEFINES

#include <time.h>
#include <iostream>
#include <math.h>
#include "Vec3.h"
#include "Vec2.h"
#include "Mat2.h"
#include "Mat4.h"
#include "Mat3.h"

// TODO Reduce the number of casts from integer to float (in situations like: 2 should be 2.0f)

int main()
{

	//CHALLENGE

	Vec4 v(0.5f, 0, 0, 1);
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

	//srand((unsigned int)time(NULL));

	/*Mat4 in1;
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
	/*
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
	/*
	std::cout << "\n-------DUAL-----\n" << Mat3::dual(Vec3(1, 2, 3));
	
	std::cout << "\n-------SCA_S------\n" << Mat4::scaling(3);
	std::cout << "\n-------SCA------\n" << Mat4::scaling(1, 2, 3);
	std::cout << "\n-------TRNSLT------\n" << Mat4::translation(1, 2 , 3);
	std::cout << "\n-------ROT------\n" << Mat4::rotation(M_PI_2, Vec3::X);
	*/

	
}



