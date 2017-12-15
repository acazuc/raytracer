#include "Fog.h"
#include <algorithm>
#include <cmath>

static Vec4 getPixelAt(Vec4 *img, uint64_t x, uint64_t y, uint64_t width, uint64_t height)
{
	x = std::min(width - 1, std::max((uint64_t)0, x));
	y = std::min(height - 1, std::max((uint64_t)0, y));
	return (img[x + y * width]);
}

static float getZIndexAt(float *zbuffer, uint64_t x, uint64_t y, uint64_t width, uint64_t height)
{
	x = std::min(width - 1, std::max((uint64_t)0, x));
	y = std::min(height - 1, std::max((uint64_t)0, y));
	return (zbuffer[x + y * width]);
}

static Vec4 process(Vec4 *img, float *zbuffer, Vec2 &pos, uint64_t width, uint64_t height, Vec4 color, float start, float density, enum FogType type)
{
	Vec4 diffuse = getPixelAt(img, pos.x, pos.y, width, height);
	float z = getZIndexAt(zbuffer, pos.x, pos.y, width, height);
	float factor = 0;
	switch (type)
	{
		case FOG_LINEAR:
			factor = (z - start) / (density - start);
			break;
		case FOG_EXP:
		default:
			factor = exp(-(density - (z - start)));
			break;
		case FOG_EXP2:
			float a = density - (z - start);
			factor = exp(-a * a);
			break;
	}
	factor = std::min(1.f, std::max(0.f, factor));
	return (diffuse * (1 - factor) + color * factor);
}

Vec4 *Fog::fog(Vec4 *img, float *zbuffer, uint64_t width, uint64_t height, Vec4 color, float start, float density, enum FogType type)
{
	Vec4 *newImg = new Vec4[width * height];
	for (uint64_t x = 0; x < width; ++x)
	{
		for (uint64_t y = 0; y < height; ++y)
		{
			Vec2 pos(x, y);
			newImg[x + y * width] = process(img, zbuffer, pos, width, height, color, start, density, type);
		}
	}
	return (newImg);
}
