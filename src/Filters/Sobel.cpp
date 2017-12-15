#include "Sobel.h"
#include <algorithm>
#include <cmath>

static Vec3 sx[3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
static Vec3 sy[3] = {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};

static Vec4 getPixelAt(Vec4 *img, uint64_t x, uint64_t y, uint64_t width, uint64_t height)
{
	x = std::min(width - 1, std::max((uint64_t)0, x));
	y = std::min(height - 1, std::max((uint64_t)0, y));
	return (img[x + y * width]);
}

static float getZIndexAt(float *zbuffer, uint64_t x, uint64_t y, uint64_t width, uint64_t height)
{
	x = std::min(width - 1, std::max((uint64_t)0, x));
	y = std::min(height - 1, std::max((uint64_t)0, y));
	return (zbuffer[x + y * width]);
}

static Vec4 process(Vec4 *img, float *zbuffer, Vec2 &pos, uint64_t width, uint64_t height)
{
	Vec4 diffuse = getPixelAt(img, pos.x, pos.y, width, height);
	Vec3 data[3];
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			//Vec3 sample(getZIndexAt(zbuffer, pos.x + i - 1, pos.y + j - 1, width, height) / 50);
			data[i][j] = getZIndexAt(zbuffer, pos.x + i - 1, pos.y + j - 1, width, height);//sample.length();
		}
	}
	float gx = sx[0].dot(data[0]) + sx[1].dot(data[1]) + sx[2].dot(data[2]);
	float gy = sy[0].dot(data[0]) + sy[1].dot(data[1]) + sy[2].dot(data[2]);
	float g = sqrt(gx * gx + gy * gy);
	return (Vec4(diffuse.rgb() - Vec3(g), diffuse.a));
}

Vec4 *Sobel::sobel(Vec4 *img, float *zbuffer, uint64_t width, uint64_t height)
{
	Vec4 *newImg = new Vec4[width * height];
	for (uint64_t x = 0; x < width; ++x)
	{
		for (uint64_t y = 0; y < height; ++y)
		{
			Vec2 pos(x, y);
			newImg[x + y * width] = process(img, zbuffer, pos, width, height);
		}
	}
	return (newImg);
}
