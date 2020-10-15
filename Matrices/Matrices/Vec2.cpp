#include <math.h>
#include "Vec2.h"
#include "MathAux.h"

Vec2::Vec2() : Vec2(0) { }
Vec2::Vec2(float xy) : Vec2(xy,xy) { }
Vec2::Vec2(float x, float y) : x(x), y(y) { }

Vec2 Vec2::operator+(const Vec2& other) const {
	return Vec2(this->x + other.x, this->y + other.y);
}

Vec2 Vec2::operator-(const Vec2& other) const {
	return Vec2(this->x - other.x, this->y - other.y);
}

Vec2& Vec2::operator=(const Vec2& other) {
	this->x = other.x;
	this->y = other.y;
	return *this;
}

Vec2& Vec2::operator+=(const Vec2& other) {
	this->x += other.x;
	this->y += other.y;
	return *this;
}

Vec2& Vec2::operator-=(const Vec2& other) {
	this->x -= other.x;
	this->y -= other.y;
	return *this;
}


Vec2 Vec2::operator*(float scalar) const {
	return Vec2(this->x * scalar, this->y * scalar);
}

Vec2 Vec2::operator/(float scalar) const {
	return Vec2(this->x / scalar, this->y / scalar);
}

Vec2 Vec2::operator+(float scalar) const {
	return Vec2(this->x + scalar, this->y + scalar);
}

Vec2 Vec2::operator-(float scalar) const {
	return Vec2(this->x - scalar, this->y - scalar);
}

Vec2& Vec2::operator+=(float s) {
	this->x += s;
	this->y += s;
	return *this;
}

Vec2& Vec2::operator-=(float s) {
	this->x -= s;
	this->y -= s;
	return *this;
}

Vec2& Vec2::operator*=(float s) {
	this->x *= s;
	this->y *= s;
	return *this;
}

Vec2& Vec2::operator/=(float s) {
	this->x /= s;
	this->y /= s;
	return *this;
}

bool Vec2::operator==(const Vec2& other) const {
	return cmpf(this->x,other.x) && cmpf(this->y, other.y);
}

bool Vec2::operator!=(const Vec2& other) const {
	return !(*this == other);
}

float Vec2::magnitude() const {
	return sqrt(this->x * this->x + this->y * this->y);
}

float Vec2::sqrMagnitude() const {
	return this->x * this->x + this->y * this->y;
}

//TODO We should consider if this returns a new vector, or if it normalizes the current vector
Vec2 Vec2::normalize() const {
	return (*this)/(this->magnitude());
}

Vec3 Vec2::toVec3() const {
	return Vec3(this->x, this->y, 0);
}

Vec4 Vec2::toVec4() const {
	return Vec4(this->x, this->y, 0, 0);
}


float dot(const Vec2& a, const Vec2& b) {
	return a.x * b.x + a.y * b.y;
}

std::ostream& operator<<(std::ostream& os, const Vec2& vec2) {
	return os << "(" << vec2.x << "," <<  vec2.y << ')';
}

Vec2 operator*(float scalar, const Vec2& vec2) {
	return Vec2(scalar * vec2.x, scalar * vec2.y);
}

Vec2 operator+(float scalar, const Vec2& vec2) {
	return Vec2(scalar + vec2.x, scalar + vec2.y);
}

Vec2 operator-(float scalar, const Vec2& vec2) {
	return Vec2(scalar - vec2.x, scalar - vec2.y);
}