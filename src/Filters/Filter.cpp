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

Vec3 Filter::rgbToHsv(Vec3 rgb)
{
	Vec3 hsv;
	float max = std::max(rgb.r, std::max(rgb.g, rgb.b));
	float min = std::min(rgb.r, std::min(rgb.g, rgb.b));
	hsv.b = max;
	float delta = max - min;
	if (delta < 0.000001)
		return Vec3(0, 0, max);
	if (max == 0)
		return Vec3(0, 0, max);
	hsv.g = delta / max;
	if (rgb.r >= max)
		hsv.r = 1 / 6. * (0 + (rgb.g - rgb.b) / delta);
	else if (rgb.g >= max)
		hsv.r = 1. / 6 * (2 + (rgb.b - rgb.r) / delta);
	else
		hsv.r = 1. / 6 * (4 + (rgb.r - rgb.g) / delta);
	if (hsv.r < 0)
		hsv.r++;
	return hsv;
}

Vec3 Filter::hsvToRgb(Vec3 hsv)
{
	float c = hsv.b * hsv.g;
	float h = hsv.r * 6;
	float x = c * (1 - std::fabs(std::fmod(hsv.r * 6, 2) - 1));
	float m = hsv.b - c;
	switch ((int)h)
	{
		case 0:
			return Vec3(c, x, 0) + m;
		case 1:
			return Vec3(x, c, 0) + m;
		case 2:
			return Vec3(0, c, x) + m;
		case 3:
			return Vec3(0, x, c) + m;
		case 4:
			return Vec3(x, 0, c) + m;
		case 5:
		default:
			return Vec3(c, 0, x) + m;
	}
}
