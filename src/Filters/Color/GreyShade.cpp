#include "GreyShade.h"

Vec4 GreyShade::operator()(Vec4 *colorBuffer, float *zBuffer, size_t x, size_t y, size_t width, size_t height)
{
	Vec4 val = colorBuffer[x + y * width];
	return Vec4(Vec3((val.r + val.g + val.b) / 3.f), val.a);
	(void)zBuffer;
	(void)height;
}
