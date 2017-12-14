#include "Cel.h"
#include <algorithm>

static Vec4 getPixelAt(Vec4 *img, uint64_t x, uint64_t y, uint64_t width, uint64_t height)
{
	x = std::min(width - 1, std::max((uint64_t)0, x));
	y = std::min(height - 1, std::max((uint64_t)0, y));
	return (img[x + y * width]);
}

Vec4 *Cel::cel(Vec4 *img, uint8_t cels, uint64_t width, uint64_t height)
{
	Vec4 *newImg = new Vec4[width * height];
	for (uint64_t x = 0; x < width; ++x)
	{
		for (uint64_t y = 0; y < height; ++y)
		{
			Vec4 val = getPixelAt(img, x, y, width, height);
			uint32_t idx = x + y * width;
			newImg[idx].x = (std::min((uint64_t)0xff, std::max((uint64_t)0, (uint64_t)(val.x * 0xff))) / cels * cels) / (float)0xff;
			newImg[idx].y = (std::min((uint64_t)0xff, std::max((uint64_t)0, (uint64_t)(val.y * 0xff))) / cels * cels) / (float)0xff;
			newImg[idx].z = (std::min((uint64_t)0xff, std::max((uint64_t)0, (uint64_t)(val.z * 0xff))) / cels * cels) / (float)0xff;
			newImg[idx].a = val.a;
		}
	}
	return (newImg);
}
