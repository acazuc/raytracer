#include "Negative.h"

void Negative::operator()(Vec4 *dst, Vec4 *src, float *zBuffer, size_t width, size_t height)
{
	for (size_t x = 0; x < width; ++x)
	{
		for (size_t y = 0; y < height; ++y)
		{
			Vec4 val = src[x + y * width];
			dst[x + y * width] = Vec4(-val.rgb() + 1.f, val.a);
		}
	}
	(void)zBuffer;
}
