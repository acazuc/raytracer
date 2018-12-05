#include "Fisheye.h"
#include "Vec/Vec2.h"
#include <algorithm>
#include <cmath>

static Vec4 getPixelAt(Vec4 *img, uint64_t x, uint64_t y, uint64_t width, uint64_t height)
{
	x = std::min(width - 1, std::max((uint64_t)0, x));
	y = std::min(height - 1, std::max((uint64_t)0, y));
	return img[x + y * width];
}

Vec4 *Fisheye::fisheye(Vec4 *img, uint64_t width, uint64_t height)
{
	Vec4 *newImg = new Vec4[width * height];
	for (uint64_t x = 0; x < width; ++x)
	{
		for (uint64_t y = 0; y < height; ++y)
		{
			float aperture = 178.;
			float apertureHalf = .5 * aperture * (M_PI / 180.);
			float maxFactor = sin(apertureHalf);
			Vec2 xy = Vec2(x / (float)width, y / (float)height) * 2.f - 1.f;
			float d = length(xy);
			if (d < (2. - maxFactor))
			{
				d = length(xy * maxFactor);
				float z = sqrt(1. - d * d);
				float r = atan2(d, z) / M_PI;
				float phi = atan2(xy.y, xy.x);
				Vec2 uv(r * cos(phi) + .5, r * sin(phi) + .5);
				newImg[x + y * width] = getPixelAt(img, uv.x * width, uv.y * height, width, height);
				continue;
			}
			newImg[x + y * width] = Vec4(0);
		}
	}
	return newImg;
}
