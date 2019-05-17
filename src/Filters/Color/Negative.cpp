#include "Negative.h"

Vec4 Negative::operator()(Vec4 *colorBuffer, float *zBuffer, size_t x, size_t y, size_t width, size_t height)
{
	Vec4 val = colorBuffer[x + y * width];
	return Vec4(-val.rgb() + 1.f, val.a);
	(void)zBuffer;
	(void)height;
}
