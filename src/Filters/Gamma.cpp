#include "Gamma.h"
#include <algorithm>
#include <cmath>

static Vec4 getPixelAt(Vec4 *img, uint64_t x, uint64_t y, uint64_t width, uint64_t height)
{
	x = std::min(width - 1, std::max((uint64_t)0, x));
	y = std::min(height - 1, std::max((uint64_t)0, y));
	return (img[x + y * width]);
}

static Vec4 process(Vec4 *img, Vec2 &pos, uint64_t width, uint64_t height, float gamma)
{
	Vec4 col = getPixelAt(img, pos.x, pos.y, width, height);
	col.r = std::min(1., std::max(0., pow(col.r, gamma)));
	col.g = std::min(1., std::max(0., pow(col.g, gamma)));
	col.b = std::min(1., std::max(0., pow(col.b, gamma)));
	return (col);
}

Vec4 *Gamma::gamma(Vec4 *img, uint64_t width, uint64_t height, float gamma)
{
	Vec4 *newImg = new Vec4[width * height];
	for (uint64_t x = 0; x < width; ++x)
	{
		for (uint64_t y = 0; y < height; ++y)
		{
			Vec2 pos(x, y);
			newImg[x + y * width] = process(img, pos, width, height, gamma);
		}
	}
	return (newImg);
}
