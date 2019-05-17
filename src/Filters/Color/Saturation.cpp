#include "Saturation.h"

Saturation::Saturation(float saturation)
: saturation(saturation)
{
}

Vec4 Saturation::operator()(Vec4 *colorBuffer, float *zBuffer, size_t x, size_t y, size_t width, size_t height)
{
	Vec4 org(colorBuffer[x + y * width]);
	Vec3 hsv = rgbToHsv(org.rgb());
	hsv.g = std::clamp(hsv.g * this->saturation, 0.f, 1.f);
	return Vec4(hsvToRgb(hsv), org.a);
	(void)zBuffer;
	(void)height;
}
