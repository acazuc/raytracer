#include "Sobel.h"
#include <algorithm>
#include <cmath>
#include <iostream>

static Vec3 sx[3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
static Vec3 sy[3] = {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};

Vec4 Sobel::operator()(Vec4 *colorBuffer, float *zBuffer, size_t x, size_t y, size_t width, size_t height)
{
	Vec4 diffuse = colorBuffer[x + y * width];
	Vec3 data[3];
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			data[i][j] = getZAt(zBuffer, ssize_t(x) + i - 1, ssize_t(y) + j - 1, width, height);
		}
	}
	float gx = dot(sx[0], data[0]) + dot(sx[1], data[1]) + dot(sx[2], data[2]);
	float gy = dot(sy[0], data[0]) + dot(sy[1], data[1]) + dot(sy[2], data[2]);
	float g = std::clamp(sqrt(gx * gx + gy * gy), 0., 1.);
	return Vec4(diffuse.rgb() * (1.f - Vec3(g)), diffuse.a);
}
