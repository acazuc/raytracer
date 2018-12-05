#include "Blur.h"
#include <algorithm>

static Vec4 getPixelAt(Vec4 *img, uint64_t x, uint64_t y, uint64_t width, uint64_t height)
{
	x = std::min(width - 1, std::max((uint64_t)0, x));
	y = std::min(height - 1, std::max((uint64_t)0, y));
	return img[x + y * width];
}

static Vec4 process(Vec4 *img, uint64_t offset, uint64_t x, uint64_t y, uint64_t width, uint64_t height)
{
	Vec4 total(0);
	uint64_t count = 0;
	for (int64_t xx = -offset; xx <= (int64_t)offset; ++xx)
	{
		for (int64_t yy = -offset; yy <= (int64_t)offset; ++yy)
		{
			total += getPixelAt(img, x + xx, y + yy, width, height);
			count++;
		}
	}
	return total / (float)count;
}

Vec4 *Blur::blur(Vec4 *img, uint64_t offset, uint64_t width, uint64_t height)
{
	Vec4 *newImg = new Vec4[width * height];
	for (uint64_t x = 0; x < width; ++x)
	{
		for (uint64_t y = 0; y < height; ++y)
		{
			newImg[x + y * width] = process(img, offset, x, y, width, height);
		}
	}
	return newImg;
}
