
/*#ifndef COLOR_H
#define COLOR_H

#include "../math/Vec4.h"

struct ColorRGBA {

	Vec4 value;

	ColorHSBA toHSBA() const;

	ColorRGBA(const float red, const float green, const float blue, const float alpha);
	ColorRGBA(const float red, const float green, const float blue);

	ColorRGBA operator+(const ColorRGBA& other) const;

	static ColorRGBA RED;
	static ColorRGBA MAGENTA;
	static ColorRGBA BLUE;
	static ColorRGBA CYAN;
	static ColorRGBA GREEN;
	static ColorRGBA YELLOW;
	static ColorRGBA WHITE;
	static ColorRGBA BLACK;
	static ColorRGBA ORANGE;
	static ColorRGBA PURPLE;
};

struct ColorHSBA{

	Vec4 value;

	ColorRGBA toRGBA() const;

	ColorHSBA(const float hue, const float sat, const float bri, const float alpha);
	ColorHSBA(const float hue, const float sat, const float bri);

	static ColorHSBA RED;
	static ColorHSBA MAGENTA;
	static ColorHSBA BLUE;
	static ColorHSBA CYAN;
	static ColorHSBA GREEN;
	static ColorHSBA YELLOW;
	static ColorHSBA WHITE;
	static ColorHSBA BLACK;
	static ColorHSBA ORANGE;
	static ColorHSBA PURPLE;
};

#endif
*/