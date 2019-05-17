#include "Fog.h"
#include <algorithm>
#include <cmath>

Fog::Fog(enum FogType type, Vec3 color, float v1, float v2)
: type(type)
, color(color)
, v1(v1)
, v2(v2)
{
}

Vec4 Fog::operator()(Vec4 *colorBuffer, float *zBuffer, size_t x, size_t y, size_t width, size_t height)
{
	Vec4 diffuse = colorBuffer[x + y * width];
	float z = zBuffer[x + y * width];
	float factor = 0;
	switch (this->type)
	{
		case FOG_LINEAR: //v1: start, v2: end
			factor = (z - this->v1) / (this->v2 - this->v1);
			break;
		case FOG_EXP: //v1: start, v2: density
		default:
			factor = exp(-(this->v2 - (z - this->v1)));
			break;
		case FOG_EXP2: //v1: start, v2: density
			float a = this->v2 - (z - this->v1);
			factor = exp(-a * a);
			break;
	}
	factor = std::min(1.f, std::max(0.f, factor));
	return Vec4(mix(diffuse.rgb(), this->color, factor), diffuse.a);
	(void)height;
}
