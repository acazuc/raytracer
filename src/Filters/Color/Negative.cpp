#include "Negative.h"

Vec4 *Negative::negative(Vec4 *img, uint64_t width, uint64_t height)
{
	Vec4 *newImg = new Vec4[width * height];
	for (uint64_t x = 0; x < width; ++x)
	{
		for (uint64_t y = 0; y < height; ++y)
		{
			Vec4 val = img[x + y * width];
			newImg[x + y * width] = Vec4(-val.rgb() + 1, val.a);
		}
	}
	return (newImg);
}
