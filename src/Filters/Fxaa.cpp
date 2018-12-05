#include "Fxaa.h"
#include <algorithm>
#include <cmath>

#define FXAA_REDUCE_MIN (1.f / 128.f)
#define FXAA_SPAN_MAX 8.f
#define FXAA_REDUCE_MUL (1.f / 8.f)

static Vec4 getPixelAt(Vec4 *img, uint64_t x, uint64_t y, uint64_t width, uint64_t height)
{
	x = std::min(width - 1, std::max((uint64_t)0, x));
	y = std::min(height - 1, std::max((uint64_t)0, y));
	return img[x + y * width];
}

static Vec4 process(Vec4 *img, Vec2 &pos, uint64_t width, uint64_t height)
{
	Vec3 rgbNW = getPixelAt(img, pos.x - 1, pos.y - 1, width, height).rgb();
	Vec3 rgbNE = getPixelAt(img, pos.x + 1, pos.y - 1, width, height).rgb();
	Vec3 rgbSW = getPixelAt(img, pos.x - 1, pos.y + 1, width, height).rgb();
	Vec3 rgbSE = getPixelAt(img, pos.x + 1, pos.y + 1, width, height).rgb();
	Vec3 rgbM  = getPixelAt(img, pos.x, pos.y, width, height).rgb();
	Vec3 luma(0.299, 0.587, 0.114);
	float lumaNW = dot(rgbNW, luma);
	float lumaNE = dot(rgbNE, luma);
	float lumaSW = dot(rgbSW, luma);
	float lumaSE = dot(rgbSE, luma);
	float lumaM  = dot(rgbM, luma);
	float lumaMin = std::min(lumaM, std::min(std::min(lumaNW, lumaNE), std::min(lumaSW, lumaSE)));
	float lumaMax = std::max(lumaM, std::max(std::max(lumaNW, lumaNE), std::max(lumaSW, lumaSE)));
	Vec2 dir(-((lumaNW + lumaNE) - (lumaSW + lumaSE)), ((lumaNW + lumaSW) - (lumaNE + lumaSE)));
	float dirReduce = std::max((lumaNW + lumaNE + lumaSW + lumaSE) * (.25f * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);
	float rcpDirMin = 1. / (std::min(std::abs(dir.x), std::abs(dir.y)) + dirReduce);
	dir = clamp(dir * rcpDirMin, -FXAA_SPAN_MAX, FXAA_SPAN_MAX);
	Vec4 a1 = getPixelAt(img, (int64_t)std::round(pos.x + dir.x * (1. / 3. - .5)), (int64_t)std::round(pos.y + dir.y * (1. / 3. - .5)), width, height);
	Vec4 a2 = getPixelAt(img, (int64_t)std::round(pos.x + dir.x * (2. / 3. - .5)), (int64_t)std::round(pos.y + dir.y * (2. / 3. - .5)), width, height);
	Vec4 rgbA(a1 + a2);
	rgbA /= 2.f;
	Vec4 rgbB(rgbA / 2.f);
	Vec4 b1 = getPixelAt(img, (int64_t)std::round(pos.x + dir.x * -.5), (int64_t)std::round(pos.y + dir.y * -.5), width, height);
	Vec4 b2 = getPixelAt(img, (int64_t)std::round(pos.x + dir.x *  .5), (int64_t)std::round(pos.y + dir.y *  .5), width, height);
	rgbB += (b1 + b2) * .25f;
	float lumaB = dot(rgbB.xyz(), luma);
	if ((lumaB < lumaMin) || (lumaB > lumaMax))
		return rgbA;
	return rgbB;
}

Vec4 *Fxaa::fxaa(Vec4 *img, uint64_t width, uint64_t height)
{
	Vec4 *newImg = new Vec4[width * height];
	for (uint64_t x = 0; x < width; ++x)
	{
		for (uint64_t y = 0; y < height; ++y)
		{
			Vec2 pos(x, y);
			newImg[x + y * width] = process(img, pos, width, height);
		}
	}
	return newImg;
}
