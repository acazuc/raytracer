#include "Gamma.h"
#include <algorithm>
#include <cmath>

Gamma::Gamma(float gamma)
: gamma(gamma)
{
}

Vec4 Gamma::operator()(Vec4 *colorBuffer, float *zBuffer, size_t x, size_t y, size_t width, size_t height)
{
	Vec4 col = colorBuffer[x + y * width];
	col.r = std::min(1., std::max(0., pow(col.r, this->gamma)));
	col.g = std::min(1., std::max(0., pow(col.g, this->gamma)));
	col.b = std::min(1., std::max(0., pow(col.b, this->gamma)));
	return col;
	(void)height;
	(void)zBuffer;
}
