#include "Sepia.h"

Vec4 Sepia::operator()(Vec4 *colorBuffer, float *zBuffer, size_t x, size_t y, size_t width, size_t height)
{
	Vec4 val = colorBuffer[x + y * width];
	Vec3 vrgb = val.rgb();
	Vec4 ret;
	ret.r = dot(vrgb, Vec3(0.393, 0.769, 0.189));
	ret.g = dot(vrgb, Vec3(0.349, 0.686, 0.168));
	ret.b = dot(vrgb, Vec3(0.272, 0.534, 0.131));
	ret.a = val.a;
	return ret;
	(void)zBuffer;
	(void)height;
}
