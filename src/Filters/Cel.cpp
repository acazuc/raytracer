#include "Cel.h"
#include <cmath>

Cel::Cel(uint8_t cels)
: cels(cels)
{
}

Vec4 Cel::operator()(Vec4 *colorBuffer, float *zBuffer, size_t x, size_t y, size_t width, size_t height)
{
	Vec4 val = colorBuffer[x + y * width];
	float step = 1. / this->cels;
	Vec4 ret;
	ret.r = val.r - std::fmod(val.r, step);
	ret.g = val.g - std::fmod(val.g, step);
	ret.b = val.b - std::fmod(val.b, step);
	ret.a = val.a;
	return ret;
	(void)zBuffer;
	(void)height;
}
