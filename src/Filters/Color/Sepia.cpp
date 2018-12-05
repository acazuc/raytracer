#include "Sepia.h"

Vec4 *Sepia::sepia(Vec4 *img, uint64_t width, uint64_t height)
{
	Vec4 *newImg = new Vec4[width * height];
	for (uint64_t x = 0; x < width; ++x)
	{
		for (uint64_t y = 0; y < height; ++y)
		{
			Vec4 val = img[x + y * width];
			Vec4 &newVal = newImg[x + y * width];
			Vec3 vrgb = val.rgb();
			newVal.r = dot(vrgb, Vec3(0.393, 0.769, 0.189));
			newVal.g = dot(vrgb, Vec3(0.349, 0.686, 0.168));
			newVal.b = dot(vrgb, Vec3(0.272, 0.534, 0.131));
			newVal.a = val.a;
		}
	}
	return newImg;
}
