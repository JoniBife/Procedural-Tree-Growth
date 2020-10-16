#ifndef MATRIX_4_H
#define MATRIX_4_H


#include <iostream>
#include "Vec4.h"
#include "Vec3.h"
#include "Mat3.h"


struct Mat4 {

	float m[4][4];

	Mat4();
	Mat4(const float fill);
	Mat4(const float l1c1, const float l1c2, const float l1c3, const float l1c4,
		 const float l2c1, const float l2c2, const float l2c3, const float l2c4,
		 const float l3c1, const float l3c2, const float l3c3, const float l3c4,
		 const float l4c1, const float l4c2, const float l4c3, const float l4c4);
	Mat4(const Mat4& other);

	static Mat4 IDENTITY;
	static Mat4 scaling(const float xyz);
	static Mat4 scaling(const float x, const float y, const float z);
	static Mat4 translation(const float x, const float y, const float z);
	static Mat4 rotation(const float angleRad, const Vec3& axis);

	Mat4& operator=(const Mat4& other);
	Mat4& operator+=(const Mat4& other);
	Mat4& operator-=(const Mat4& other);
	Mat4& operator*=(const Mat4& other);

	bool operator==(const Mat4& other) const;
	bool operator!=(const Mat4& other) const;

	Mat4 operator+(const Mat4& other);
	Mat4 operator-(const Mat4& other);
	Mat4 operator*(const Mat4& other);
	Mat4 operator*(const float s);
	friend Mat4 operator*(const float s, const Mat4& mat4);
	Vec4 operator*(const Vec4& v);

	/*class Mat4Im {
	int lines;
	Mat4& mat4;
	Mat4Im(int lines, Mat4& mat4);

	float* operator[](int columns) {
		return mat4.m[lines];
	}
	};

	Mat4Im operator[](int lines) {
		return Mat4Im(lines);
	}*/
	
	Mat4 transpose();
	void toOpenGLFormat(float array[16]);

	Mat3 toMat3(); //removes last line and column

	friend std::ostream& operator<<(std::ostream& os, const Mat4& mat4);
	friend std::istream& operator>>(std::istream& is, Mat4& mat4);

};
#endif

