#include "Sepia.h"

Vec3 *Sepia::sepia(Vec3 *img, uint64_t width, uint64_t height)
{
	Vec3 *newImg = new Vec3[width * height];
	for (uint64_t x = 0; x < width; ++x)
	{
		for (uint64_t y = 0; y < height; ++y)
		{
			Vec3 val = img[x + y * width];
			Vec3 &newVal = newImg[x + y * width];
			newVal.r = val.dot(Vec3(0.393, 0.769, 0.189));
			newVal.g = val.dot(Vec3(0.349, 0.686, 0.168));
			newVal.b = val.dot(Vec3(0.272, 0.534, 0.131));
		}
	}
	return (newImg);
}
