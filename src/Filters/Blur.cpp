#include "Blur.h"
#include <algorithm>

Vec3 Blur::getPixelAt(Vec3 *img, uint64_t x, uint64_t y, uint64_t width, uint64_t height)
{
	x = std::min(width - 1, std::max((uint64_t)0, x));
	y = std::min(height - 1, std::max((uint64_t)0, y));
	return (img[x + y * width]);
}

Vec3 Blur::process(Vec3 *img, uint64_t offset, uint64_t x, uint64_t y, uint64_t width, uint64_t height)
{
	Vec3 total(0);
	uint64_t count = 0;
	for (int64_t xx = -offset; xx <= (int64_t)offset; ++xx)
	{
		for (int64_t yy = -offset; yy <= (int64_t)offset; ++yy)
		{
			total += getPixelAt(img, x + xx, y + yy, width, height);
			count++;
		}
	}
	return (total / count);
}

Vec3 *Blur::blur(Vec3 *img, uint64_t offset, uint64_t width, uint64_t height)
{
	Vec3 *newImg = new Vec3[width * height];
	for (uint64_t x = 0; x < width; ++x)
	{
		for (uint64_t y = 0; y < height; ++y)
		{
			newImg[x + y * width] = process(img, offset, x, y, width, height);
		}
	}
	return (newImg);
}
