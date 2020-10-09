#include <Math.h>
#include "Vec3.h"
#include "MathAux.h"

Vec3::Vec3() : Vec3(0) {}
Vec3::Vec3(float xyz) : Vec3(xyz, xyz, xyz) {}
Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) { }

Vec3 Vec3::ZERO = Vec3(0, 0, 0);
Vec3 Vec3::Z = Vec3(0, 0, 1);
Vec3 Vec3::Y = Vec3(0, 1, 0);
Vec3 Vec3::X = Vec3(1, 0, 0);

Vec3 Vec3::random() {
	return Vec3(randomFloat(), randomFloat(), randomFloat());
}

Vec3 Vec3::operator+(const Vec3& other) const {
	return Vec3(this->x + other.x, this->y + other.y, this->z + other.z);
}

Vec3 Vec3::operator-(const Vec3& other) const {
	return Vec3(this->x - other.x, this->y - other.y, this->z - other.z);
}

Vec3& Vec3::operator=(const Vec3& other) {
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	return *this;
}

Vec3& Vec3::operator+=(const Vec3& other) {
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
	return *this;
}

Vec3& Vec3::operator-=(const Vec3& other) {
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
	return *this;
}

Vec3 Vec3::operator*(float scalar) const {
	return Vec3(this->x * scalar, this->y * scalar, this->z * scalar);
}

Vec3 Vec3::operator/(float scalar) const {
	return Vec3(this->x / scalar, this->y / scalar, this->z / scalar);
}

Vec3 Vec3::operator+(float scalar) const {
	return Vec3(this->x + scalar, this->y + scalar, this->z + scalar);
}

Vec3 Vec3::operator-(float scalar) const {
	return Vec3(this->x - scalar, this->y - scalar, this->z - scalar);
}

Vec3& Vec3::operator+=(float s) {
	this->x += s;
	this->y += s;
	this->z += s;
	return *this;
}
Vec3& Vec3::operator-=(float s) {
	this->x -= s;
	this->y -= s;
	this->z -= s;
	return *this;
}
Vec3& Vec3::operator*=(float s) {
	this->x *= s;
	this->y *= s;
	this->z *= s;
	return *this;
}
Vec3& Vec3::operator/=(float s) {
	this->x /= s;
	this->y /= s;
	this->z /= s;
	return *this;
}


// cmpf uses compares the floats with an error (EPSILON)
bool Vec3::operator==(const Vec3& other) const {
	return cmpf(this->x , other.x) && cmpf(this->y, other.y) && cmpf(this->z, other.z);
}

bool Vec3::operator!=(const Vec3& other) const {
	return !(*this == other);
}


float Vec3::magnitude() const {
	return sqrt(this->x * this->x + this->y * this->y + this->z*this->z);
}

float Vec3::sqrMagnitude() const {
	return this->x * this->x + this->y * this->y + this->z * this->z;
}

Vec3 Vec3::normalize() const {
	return (*this) / (this->magnitude());
}

Vec4 Vec3::toVec4() const {
	return Vec4(this->x, this->y, this->z, 0);
}



float dot(const Vec3& a, const Vec3& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 cross(const Vec3& a, const Vec3& b) {
	return (a.y * b.z - a.z * b.y) * Vec3::X + (a.z * b.x - a.x * b.z) * Vec3::Y + (a.x * b.y - a.y * b.x) * Vec3::Z;
}

std::ostream& operator<<(std::ostream& os, const Vec3& vec3) {
	return os << "(" << vec3.x << "," << vec3.y << "," << vec3.z << ')';
}

Vec3 operator*(const float scalar, const Vec3& vec3) {
	return Vec3(scalar * vec3.x, scalar * vec3.y, scalar * vec3.z);
}

Vec3 operator+(float scalar, const Vec3& vec3) {
	return Vec3(scalar + vec3.x, scalar + vec3.y, scalar + vec3.z);
}

Vec3 operator-(float scalar, const Vec3& vec3) {
	return Vec3(scalar - vec3.x, scalar - vec3.y, scalar - vec3.z);
}