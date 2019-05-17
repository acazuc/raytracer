#include "Fisheye.h"
#include "Vec/Vec2.h"
#include <cmath>

Fisheye::Fisheye(float aperture)
: aperture(aperture)
{
}

Vec4 Fisheye::operator()(Vec4 *colorBuffer, float *zBuffer, size_t x, size_t y, size_t width, size_t height)
{
	float apertureHalf = .5 * this->aperture * M_PI / 180.;
	float maxFactor = std::sin(apertureHalf);
	Vec2 xy = Vec2(x / float(width), y / float(height)) * 2.f - 1.f;
	float d = length(xy);
	if (d >= (2. - maxFactor))
		return Vec4(0);
	d = length(xy * maxFactor);
	float z = std::sqrt(1. - d * d);
	float r = std::atan2(d, z) / M_PI;
	float phi = std::atan2(xy.y, xy.x);
	Vec2 uv(r * std::cos(phi) + .5, r * std::sin(phi) + .5);
	return getPixelAt(colorBuffer, uv.x * width, uv.y * height, width, height);
	(void)zBuffer;
}
