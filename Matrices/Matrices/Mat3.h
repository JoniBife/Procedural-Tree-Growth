#ifndef MATRIX_3_H
#define MATRIX_3_H


#include <iostream>
#include "Vec3.h"


struct Mat3 {

	float m[3][3];

	Mat3();
	Mat3(const float fill);
	Mat3(const float l1c1, const float l1c2, const float l1c3,
		const float l2c1, const float l2c2, const float l2c3,
		const float l3c1, const float l3c2, const float l3c3);
	Mat3(const Mat3& other);

	float determinant();

	static Mat3 IDENTITY;
	//static Mat3 scaling(const float xyz);
	//static Mat3 scaling(const float x, const float y, const float z);
	//static Mat3 translation(const float x, const float y, const float z);
	//static Mat3 rotation(const float angleRad, const Vec3& axis);

	Mat3& operator=(const Mat3& other);
	Mat3& operator+=(const Mat3& other);
	Mat3& operator-=(const Mat3& other);
	Mat3& operator*=(const Mat3& other);

	bool operator==(const Mat3& other) const;
	bool operator!=(const Mat3& other) const;

	Mat3 operator+(const Mat3& other);
	Mat3 operator-(const Mat3& other);
	Mat3 operator*(const Mat3& other);
	Mat3 operator*(const float s);
	friend Mat3 operator*(const float s, const Mat3& mat3);
	Vec3 operator*(const Vec3& v);

	Mat3 transpose();
	bool inverse(Mat3& mat3);
	static Mat3 dual(const Vec3& v);
	void toOpenGLFormat(float array[9]);

	friend std::ostream& operator<<(std::ostream& os, const Mat3& mat3);
	friend std::istream& operator>>(std::istream& is, Mat3& mat3);

};
#endif