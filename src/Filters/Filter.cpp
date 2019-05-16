#include "Filter.h"
#include <algorithm>

Vec4 Filter::getPixelAt(Vec4 *img, size_t x, size_t y, size_t width, size_t height)
{
	x = std::min(width - 1, std::max(size_t(0), x));
	y = std::min(height - 1, std::max(size_t(0), y));
	return img[x + y * width];
}

float Filter::getZAt(float *zBuffer, size_t x, size_t y, size_t width, size_t height)
{
	x = std::min(width - 1, std::max(size_t(0), x));
	y = std::min(height - 1, std::max(size_t(0), y));
	return zBuffer[x + y * width];
}
