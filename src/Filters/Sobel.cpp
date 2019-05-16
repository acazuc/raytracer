#include "Sobel.h"
#include <algorithm>
#include <cmath>
#include <iostream>

static Vec3 sx[3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
static Vec3 sy[3] = {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};

Vec4 Sobel::process(Vec4 *img, float *zBuffer, size_t x, size_t y, size_t width, size_t height)
{
	Vec4 diffuse = img[x + y * width];
	Vec3 data[3];
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			//Vec3 sample(getZAt(zBuffer, pos.x + i - 1, pos.y + j - 1, width, height) / 50);
			data[i][j] = getZAt(zBuffer, x + i - 1, y + j - 1, width, height);//sample.length();
		}
	}
	float gx = dot(sx[0], data[0]) + dot(sx[1], data[1]) + dot(sx[2], data[2]);
	float gy = dot(sy[0], data[0]) + dot(sy[1], data[1]) + dot(sy[2], data[2]);
	float g = std::clamp(sqrt(gx * gx + gy * gy), 0., 1.);
	return Vec4(diffuse.rgb() * (1.f - Vec3(g)), diffuse.a);
}

void Sobel::operator()(Vec4 *dst, Vec4 *src, float *zBuffer, size_t width, size_t height)
{
	for (size_t x = 0; x < width; ++x)
	{
		for (size_t y = 0; y < height; ++y)
		{
			dst[x + y * width] = process(src, zBuffer, x, y, width, height);
		}
	}
}
