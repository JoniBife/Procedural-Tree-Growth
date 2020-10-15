#ifndef VEC2_H
#define VEC2_H

#include <iostream>
#include "Vec3.h"
#include "Vec4.h"

class Vec2 {

public: 
	float x, y;

	// TODO Define some constants such as UP Vector, DOWN Vector, etc...

	Vec2();
	Vec2(float xy);
	Vec2(float x, float y);

	Vec2 operator+(const Vec2& other) const;
	Vec2 operator-(const Vec2& other) const;
	Vec2& operator=(const Vec2& other);
	Vec2& operator+=(const Vec2& other);
	Vec2& operator-=(const Vec2& other);

	Vec2 operator*(float scalar) const;
	friend Vec2 operator*(float scalar, const Vec2& vec2);
	Vec2 operator/(float scalar) const;
	Vec2 operator+(float scalar) const;
	friend Vec2 operator+(float scalar, const Vec2& vec2);
	Vec2 operator-(float scalar) const;
	friend Vec2 operator-(float scalar, const Vec2& vec2);

	Vec2& operator+=(float s);
	Vec2& operator-=(float s);
	Vec2& operator*=(float s);
	Vec2& operator/=(float s);

	// Should compare the vector components (x,y)
	bool operator==(const Vec2& other) const;
	bool operator!=(const Vec2& other) const;

	float magnitude() const;
	float sqrMagnitude() const;
	Vec2 normalize() const;

	Vec3 toVec3() const;
	Vec4 toVec4() const;
};

float dot(const Vec2& a, const Vec2& b);
std::ostream& operator<<(std::ostream& os, const Vec2& vec2);

#endif