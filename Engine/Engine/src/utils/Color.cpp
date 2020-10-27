/*#include "Color.h"
#include <math.h>

ColorHSBA ColorRGBA::toHSBA() const {

	float hue, sat, bri;

	float max = fmaxf(value.x, value.y);
	max = fmaxf(max, value.z);

	float min = fminf(value.x, value.y);
	min = fminf(max, value.z);
	float min2;

	if (min = value.x) {
		min2 = fminf(value.y, value.z);
	}
	else if (min = value.y) {
		min2 = fminf(value.x, value.z);
	}
	else min2 = fminf(value.x, value.y);

	hue = (min2 - min) * 360;

	sat = (max - min) / max;

	bri = max;

	return ColorHSBA(hue, sat, bri, value.w);

}

ColorRGBA::ColorRGBA(const float red, const float green, const float blue, const float alpha) : value(Vec4(red, green, blue, alpha)) {}

ColorRGBA::ColorRGBA(const float red, const float green, const float blue) : value(Vec4(red, green, blue, 1.0f)) {}

ColorRGBA ColorRGBA::RED = { 1.0f, 0.0f, 0.0f, 1.0f };
ColorRGBA ColorRGBA::BLUE = { 0.0f, 0.0f, 1.0f, 1.0f };
ColorRGBA ColorRGBA::CYAN = { 0.0f, 1.0f, 1.0f, 1.0f };
ColorRGBA ColorRGBA::GREEN = { 0.0f, 1.0f, 0.0f, 1.0f };
ColorRGBA ColorRGBA::YELLOW = { 1.0f, 1.0f, 0.0f, 1.0f };
ColorRGBA ColorRGBA::MAGENTA = { 1.0f, 0.0f, 1.0f, 1.0f };
ColorRGBA ColorRGBA::WHITE = { 1.0f, 1.0f, 1.0f, 1.0f };
ColorRGBA ColorRGBA::BLACK = { 0.0f, 0.0f, 0.0f, 1.0f };
ColorRGBA ColorRGBA::ORANGE = { 1.0f, 0.5f, 0.0f, 1.0f };
ColorRGBA ColorRGBA::PURPLE = { 0.5f, 0.0f, 1.0f, 1.0f };

ColorRGBA ColorRGBA::operator+(const ColorRGBA& other) const {

	Vec4 newColor = value + other.value;

	if (newColor.x > 1.0f) newColor.x = 1.0f;
	if (newColor.y > 1.0f) newColor.y = 1.0f;
	if (newColor.z > 1.0f) newColor.z = 1.0f;
	if (newColor.w > 1.0f) newColor.w = 1.0f;

	return ColorRGBA(newColor.x, newColor.y, newColor.z, newColor.w);
}

ColorRGBA ColorHSBA::toRGBA() const {
	// TODO
	return ColorRGBA::BLACK;
}

ColorHSBA::ColorHSBA(const float hue, const float sat, const float bri, const float alpha) : value(Vec4(hue, sat, bri, alpha)) {}
ColorHSBA::ColorHSBA(const float hue, const float sat, const float bri) : value(Vec4(hue, sat, bri, 1.0f)) {}

*/