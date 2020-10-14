#include "Mat4.h"
#include "MathAux.h"

Mat4::Mat4() : Mat4(0) {}

Mat4::Mat4(const float fill) : Mat4(fill, fill, fill, fill, fill, fill, fill, fill, fill, fill, fill, fill, fill, fill, fill, fill) {}

Mat4::Mat4(const float l1c1, const float l1c2, const float l1c3, const float l1c4,
	const float l2c1, const float l2c2, const float l2c3, const float l2c4,
	const float l3c1, const float l3c2, const float l3c3, const float l3c4,
	const float l4c1, const float l4c2, const float l4c3, const float l4c4) {

	m[0][0] = l1c1;
	m[0][1] = l1c2;
	m[0][2] = l1c3;
	m[0][3] = l1c4;
	m[1][0] = l2c1;
	m[1][1] = l2c2;
	m[1][2] = l2c3;
	m[1][3] = l2c4;
	m[2][0] = l3c1;
	m[2][1] = l3c2;
	m[2][2] = l3c3;
	m[2][3] = l3c4;
	m[3][0] = l4c1;
	m[3][1] = l4c2;
	m[3][2] = l4c3;
	m[3][3] = l4c4;
}

Mat4::Mat4(const Mat4& other) {
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			m[l][c] = other.m[l][c];
		}
	}
}


Mat4 Mat4::IDENTITY = {1, 0, 0, 0,
						0, 1, 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1};

Mat4 Mat4::scaling(const float xyz)
{
	return scaling(xyz, xyz, xyz);
}

Mat4 Mat4::scaling(const float x, const float y, const float z)
{
	return {x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1};
}

Mat4 Mat4::translation(const float x, const float y, const float z)
{
	return {1, 0, 0, x,
			0, 1, 0, y,
			0, 0, 1, z,
			0, 0, 0, 1};
}
// TODO static Mat4 rotation(float)

Mat4& Mat4::operator=(const Mat4& other)
{
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			m[l][c] = other.m[l][c];			
		}
	}
	return *this;
}

Mat4& Mat4::operator+=(const Mat4& other)
{
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			m[l][c] += other.get(l, c);
		}
	}
	return *this;
}

Mat4& Mat4::operator-=(const Mat4& other)
{
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			m[l][c] -= other.get(l, c);
		}
	}
	return *this;
}

Mat4& Mat4::operator*=(const Mat4& other)
{
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			m[l][c] = m[l][0]*other.get(0, l) + m[l][1]*other.get(1, l) + m[l][2]*other.get(2, l) + m[l][3]*other.get(3, l);
		}
	}
}

bool Mat4::operator==(const Mat4& other) const {
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			if (!cmpf(m[l][c], other.get(l, c)))
				return false;
		}
	}
	return true;
}

bool Mat4::operator!=(const Mat4& other) const {
	return !(*this == other);
}

Mat4 Mat4::operator+(const Mat4& other) {
	Mat4 sum;
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			sum.m[l][c] = m[l][c] + other.m[l][c];
		}
	}
	return sum;
}
Mat4 Mat4::operator-(const Mat4& other) {
	Mat4 diff;
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			diff.m[l][c] = m[l][c] + other.m[l][c];
		}
	}
	return diff;
}
Mat4 Mat4::operator*(const Mat4& other) {
	Mat4 prod;
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			prod.m[l][c] = m[l][0] * other.m[0][l] + 
						   m[l][1] * other.m[1][l] + 
						   m[l][2] * other.m[2][l] + 
						   m[l][3] * other.m[3][l];
		}
	}
	return prod;
}
Mat4 Mat4::operator*(const float s) {
	Mat4 prod;
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			prod.m[l][c] *= m[l][c] * s;
		}
	}
	return prod;
}


Mat4 operator*(const float s, const Mat4& mat4) {
	Mat4 prod;
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			prod.m[l][c] = mat4.m[l][c] * s;
		}
	}
	return prod;
}
Vec4 Mat4::operator*(const Vec4& v) {
	Vec4 prod;
	prod.x = m[0][0]*v.x;
	prod.y = m[1][1]*v.y;
	prod.z = m[2][2]*v.z;
	return prod;
}

Mat4 Mat4::transpose() {
	Mat4 trans;
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			trans.m[l][c] = m[c][l];
		}
	}
	return trans;
}
// TODO CONVERT TO OPENGL COLUMN MAJOR


/*
 * Print result example:
 * [ 1 , 0 , 0 , 0 ]
 * [ 0 , 1 , 0 , 0 ]
 * [ 0 , 0 , 1 , 0 ]
 * [ 0 , 0 , 0 , 1 ]
*/
std::ostream& operator<<(std::ostream& os, const Mat4& mat4) {
	for (int l = 0; l < 4; ++l) {
		os << "[ ";
		for (int c = 0; c < 4; ++c) {
			os << mat4.m[l][c];
			if (c < 3) { // Only print comma until the third column
				os << " , ";
			}
		}
		os << " ]" << std::endl;
	}
	return os;
}