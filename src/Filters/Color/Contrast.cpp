#include "Contrast.h"

Contrast::Contrast(float contrast)
: contrast(contrast)
{
}

Vec4 Contrast::operator()(Vec4 *colorBuffer, float *zBuffer, size_t x, size_t y, size_t width, size_t height)
{
	Vec4 org(colorBuffer[x + y * width]);
	return Vec4(clamp((org.rgb() - .5f) * this->contrast + .5f, 0.f, 1.f), org.a);
	(void)zBuffer;
	(void)height;
}
