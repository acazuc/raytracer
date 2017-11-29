#include "Cel.h"
#include <algorithm>

Vec3 Cel::getPixelAt(Vec3 *img, uint64_t x, uint64_t y, uint64_t width, uint64_t height)
{
	x = std::min(width - 1, std::max((uint64_t)0, x));
	y = std::min(height - 1, std::max((uint64_t)0, y));
	return (img[x + y * width]);
}

Vec3 *Cel::cel(Vec3 *img, uint8_t cels, uint64_t width, uint64_t height)
{
	Vec3 *newImg = new Vec3[width * height];
	for (uint64_t x = 0; x < width; ++x)
	{
		for (uint64_t y = 0; y < height; ++y)
		{
			Vec3 val = getPixelAt(img, x, y, width, height);
			newImg[x + y * width].x = (std::min((uint64_t)0xff, std::max((uint64_t)0, (uint64_t)(val.x * 0xff))) / cels * cels) / (float)0xff;
			newImg[x + y * width].y = (std::min((uint64_t)0xff, std::max((uint64_t)0, (uint64_t)(val.y * 0xff))) / cels * cels) / (float)0xff;
			newImg[x + y * width].z = (std::min((uint64_t)0xff, std::max((uint64_t)0, (uint64_t)(val.z * 0xff))) / cels * cels) / (float)0xff;
		}
	}
	return (newImg);
}
