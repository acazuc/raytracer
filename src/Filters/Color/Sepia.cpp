#include "Sepia.h"

void Sepia::operator()(Vec4 *dst, Vec4 *src, float *zBuffer, size_t width, size_t height)
{
	for (size_t x = 0; x < width; ++x)
	{
		for (size_t y = 0; y < height; ++y)
		{
			Vec4 val = src[x + y * width];
			Vec4 &newVal = dst[x + y * width];
			Vec3 vrgb = val.rgb();
			newVal.r = dot(vrgb, Vec3(0.393, 0.769, 0.189));
			newVal.g = dot(vrgb, Vec3(0.349, 0.686, 0.168));
			newVal.b = dot(vrgb, Vec3(0.272, 0.534, 0.131));
			newVal.a = val.a;
		}
	}
	(void)zBuffer;
}
