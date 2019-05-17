#include "Brightness.h"

Brightness::Brightness(float brightness)
: brightness(brightness)
{
	this->brightness = std::clamp(this->brightness, -1.f, 1.f);
}

Vec4 Brightness::operator()(Vec4 *colorBuffer, float *zBuffer, size_t x, size_t y, size_t width, size_t height)
{
	Vec4 org(colorBuffer[x + y * width]);
	return Vec4(clamp(org.rgb() + this->brightness, 0.f, 1.f), org.a);
	(void)zBuffer;
	(void)height;
}
