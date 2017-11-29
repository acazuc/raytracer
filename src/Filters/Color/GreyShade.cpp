#include "GreyShade.h"

Vec3 *GreyShade::greyShade(Vec3 *img, uint64_t width, uint64_t height)
{
	Vec3 *newImg = new Vec3[width * height];
	for (uint64_t x = 0; x < width; ++x)
	{
		for (uint64_t y = 0; y < height; ++y)
		{
			Vec3 val = img[x + y * width];
			newImg[x + y * width] = (val.x + val.y + val.z) / 3;
		}
	}
	return (newImg);
}
