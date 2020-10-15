#ifndef MAT_2_H
#define MAT_2_H

#include "Vec2.h"

struct Mat2 {
	float m[2][2];

	Mat2();
	Mat2(const float fill);
	Mat2(const float l1c1, const float l1c2,
		const float l2c1, const float l2c2);
	Mat2(const Mat2& other);

	static Mat2 IDENTITY;

	Mat2& operator=(const Mat2& other);
	Mat2& operator+=(const Mat2& other);
	Mat2& operator-=(const Mat2& other);
	Mat2& operator*=(const Mat2& other);
	Mat2& operator*=(const float s);
	Mat2& operator/=(const float s);

	bool operator==(const Mat2& other) const;
	bool operator!=(const Mat2& other) const;

	Mat2 operator+(const Mat2& other);
	Mat2 operator-(const Mat2& other);
	Mat2 operator*(const Mat2& other);
	Mat2 operator*(const float s);
	friend Mat2 operator*(const float s, const Mat2& mat2);
	Mat2 operator/(const float s);
	Vec2 operator*(const Vec2& v);

	/*class Mat4Im {
	int lines;
	Mat4& mat4;
	Mat4Im(int lines, Mat4& mat4);

	float operator[](int columns) {
		return mat4.m[lines][columns];
	}
	};

	Mat4Im operator[](int lines) {
		return Mat4Im(lines);
	}*/

	Mat2 transpose();
	/*
	* Returns false if this is not invertible
	* else true;
	* @param inverse will contain the inverse matrix of this
	*/
	bool inverse(Mat2& inverse);
	float determinant();
	float* toOpenGLFormat();

	/*
	 * Print result example:
	* [ 1 , 0 ]
	* [ 0 , 1 ]
	*/
	friend std::ostream& operator<<(std::ostream& os, const Mat2& mat2);
	friend std::istream& operator>>(std::istream& is, Mat2& mat2);
};

#endif
