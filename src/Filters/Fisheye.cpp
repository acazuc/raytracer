#include "Fisheye.h"
#include "Vec/Vec2.h"
#include <algorithm>
#include <cmath>

Vec3 Fisheye::getPixelAt(Vec3 *img, uint64_t x, uint64_t y, uint64_t width, uint64_t height)
{
	x = std::min(width - 1, std::max((uint64_t)0, x));
	y = std::min(height - 1, std::max((uint64_t)0, y));
	return (img[x + y * width]);
}

Vec3 *Fisheye::fisheye(Vec3 *img, uint64_t width, uint64_t height)
{
	Vec3 *newImg = new Vec3[width * height];
	for (uint64_t x = 0; x < width; ++x)
	{
		for (uint64_t y = 0; y < height; ++y)
		{
			float aperture = 178.;
			float apertureHalf = .5 * aperture * (M_PI / 180.);
			float maxFactor = sin(apertureHalf);
			Vec2 xy = Vec2(x / (float)width, y / (float)height) * 2. - 1.;
			float d = xy.length();
			if (d < (2. - maxFactor))
			{
				d = (xy * maxFactor).length();
				float z = sqrt(1. - d * d);
				float r = atan2(d, z) / M_PI;
				float phi = atan2(xy.y, xy.x);
				Vec2 uv(r * cos(phi) + .5, r * sin(phi) + .5);
				newImg[x + y * width] = getPixelAt(img, uv.x * width, uv.y * height, width, height);
				continue;
			}
			newImg[x + y * width] = Vec3(0, 0, 0);
		}
	}
	return (newImg);
}
