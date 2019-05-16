#include "Blur.h"

Blur::Blur(size_t radius)
: radius(radius)
{
}

Vec4 Blur::process(Vec4 *img, size_t x, size_t y, size_t width, size_t height)
{
	Vec4 total(0);
	size_t count = 0;
	for (ssize_t xx = -this->radius; xx <= ssize_t(this->radius); ++xx)
	{
		for (ssize_t yy = -this->radius; yy <= ssize_t(this->radius); ++yy)
		{
			total += getPixelAt(img, x + xx, y + yy, width, height);
			count++;
		}
	}
	return total / float(count);
}

void Blur::operator()(Vec4 *dst, Vec4 *src, float *zBuffer, size_t width, size_t height)
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
