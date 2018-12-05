#include "GreyShade.h"

Vec4 *GreyShade::greyShade(Vec4 *img, uint64_t width, uint64_t height)
{
	Vec4 *newImg = new Vec4[width * height];
	for (uint64_t x = 0; x < width; ++x)
	{
		for (uint64_t y = 0; y < height; ++y)
		{
			Vec4 val = img[x + y * width];
			newImg[x + y * width] = Vec4((val.x + val.y + val.z) / 3, val.a);
		}
	}
	return newImg;
}
