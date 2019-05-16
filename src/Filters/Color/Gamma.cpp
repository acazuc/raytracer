#include "Gamma.h"
#include <algorithm>
#include <cmath>

Vec4 Gamma::process(Vec4 *img, size_t x, size_t y, size_t width, size_t height)
{
	Vec4 col = img[x + y * width];
	col.r = std::min(1., std::max(0., pow(col.r, this->gamma)));
	col.g = std::min(1., std::max(0., pow(col.g, this->gamma)));
	col.b = std::min(1., std::max(0., pow(col.b, this->gamma)));
	return col;
	(void)height;
}

void Gamma::operator()(Vec4 *dst, Vec4 *src, float *zBuffer, size_t width, size_t height)
{
	for (size_t x = 0; x < width; ++x)
	{
		for (size_t y = 0; y < height; ++y)
		{
			dst[x + y * width] = process(src, x, y, width, height);
		}
	}
	(void)zBuffer;
}
