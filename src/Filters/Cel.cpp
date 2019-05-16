#include "Cel.h"
#include <cmath>

Cel::Cel(uint8_t cels)
: cels(cels)
{
}

void Cel::operator()(Vec4 *dst, Vec4 *src, float *zBuffer, size_t width, size_t height)
{
	for (size_t x = 0; x < width; ++x)
	{
		for (size_t y = 0; y < height; ++y)
		{
			Vec4 val = src[x + y * width];
			size_t idx = x + y * width;
			float step = 1. / this->cels;
			dst[idx].r = val.r - std::fmod(val.r, step);
			dst[idx].g = val.g - std::fmod(val.g, step);
			dst[idx].b = val.b - std::fmod(val.b, step);
			dst[idx].a = val.a;
		}
	}
	(void)zBuffer;
}
