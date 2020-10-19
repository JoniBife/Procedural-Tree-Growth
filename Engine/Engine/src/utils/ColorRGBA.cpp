#include "ColorRGBA.h"


Vec4 ColorRGBA::RED = Vec4(1.0f, 0.0f, 0.0f, 1.0f);
Vec4 ColorRGBA::BLUE = Vec4(0.0f, 0.0f, 1.0f, 1.0f);
Vec4 ColorRGBA::GREEN = Vec4(0.0f, 1.0f, 0.0f, 1.0f);
Vec4 ColorRGBA::YELLOW = RED + GREEN;
Vec4 ColorRGBA::MAGENTA = RED + BLUE;
Vec4 ColorRGBA::WHITE = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
Vec4 ColorRGBA::BLACK = Vec4(0.0f, 0.0f, 0.0f, 1.0f);