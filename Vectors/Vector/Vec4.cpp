#include <Math.h>
#include "Vec4.h"
#include "MathAux.h"

Vec4::Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }

Vec4 Vec4::operator+(const Vec4& other) const {
	return Vec4(this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w);
}

Vec4 Vec4::operator-(const Vec4& other) const {
	return Vec4(this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w);
}

Vec4& Vec4::operator=(const Vec4& other) {
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	this->w = other.w;
	return *this;
}

Vec4& Vec4::operator+=(const Vec4& other) {
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
	this->w += other.w;
	return *this;
}

Vec4& Vec4::operator-=(const Vec4& other) {
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
	this->w -= other.w;
	return *this;
}



Vec4 Vec4::operator*(float scalar) const {
	return Vec4(this->x * scalar, this->y * scalar, this->z * scalar, this->w * scalar);
}

Vec4 Vec4::operator/(float scalar) const {
	return Vec4(this->x / scalar, this->y / scalar, this->z / scalar, this->w / scalar);
}

Vec4 Vec4::operator+(float scalar) const {
	return Vec4(this->x + scalar, this->y + scalar, this->z + scalar, this->w + scalar);
}

Vec4 Vec4::operator-(float scalar) const {
	return Vec4(this->x - scalar, this->y - scalar, this->z - scalar, this->w - scalar);
}

Vec4& Vec4::operator+=(float s) {
	this->x += s;
	this->y += s;
	this->z += s;
	this->w += s;
	return *this;
}

Vec4& Vec4::operator-=(float s) {
	this->x -= s;
	this->y -= s;
	this->z -= s;
	this->w -= s;
	return *this;
}

Vec4& Vec4::operator*=(float s) {
	this->x *= s;
	this->y *= s;
	this->z *= s;
	this->w *= s;
	return *this;
}

Vec4& Vec4::operator/=(float s) {
	this->x /= s;
	this->y /= s;
	this->z /= s;
	this->w /= s;
	return *this;
}



bool Vec4::operator==(const Vec4& other) const {
	return cmpf(this->x, other.x) && cmpf(this->y, other.y) && cmpf(this->z, other.z) && cmpf(this->w, other.w);
}

bool Vec4::operator!=(const Vec4& other) const {
	return !(*this == other);
}



float Vec4::magnitude() const {
	return sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w*this->w);
}

float Vec4::sqrMagnitude() const {
	return this->x * this->x + this->y * this->y + this->z * this->z + this->w* this->w;
}

Vec4 Vec4::normalize() const {
	return (*this) / (this->magnitude());
}

float dot(const Vec4& a, const Vec4& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

std::ostream& operator<<(std::ostream& os, const Vec4& vec4) {
	return os << "(" << vec4.x << "," << vec4.y << "," << vec4.z << "," << vec4.w << ')';
}

Vec4 operator*(float scalar, const Vec4& vec4) {
	return Vec4(scalar * vec4.x, scalar * vec4.y, scalar * vec4.z, scalar * vec4.w);
}

Vec4 operator+(float scalar, const Vec4& vec4) {
	return Vec4(scalar + vec4.x, scalar + vec4.y, scalar + vec4.z, scalar + vec4.w);
}

Vec4 operator-(float scalar, const Vec4& vec4) {
	return Vec4(scalar - vec4.x, scalar - vec4.y, scalar - vec4.z, scalar - vec4.w);
}