#include "Fxaa.h"
#include <algorithm>
#include <cmath>

#define FXAA_REDUCE_MIN (1.f / 128.f)
#define FXAA_SPAN_MAX 8.f
#define FXAA_REDUCE_MUL (1.f / 8.f)

Vec3 Fxaa::getPixelAt(Vec3 *img, uint64_t x, uint64_t y, uint64_t width, uint64_t height)
{
	x = std::min(width - 1, std::max((uint64_t)0, x));
	y = std::min(height - 1, std::max((uint64_t)0, y));
	return (img[x + y * width]);
}

Vec3 Fxaa::process(Vec3 *img, Vec2 &pos, uint64_t width, uint64_t height)
{
	Vec3 rgbNW = getPixelAt(img, pos.x - 1, pos.y - 1, width, height);
	Vec3 rgbNE = getPixelAt(img, pos.x + 1, pos.y - 1, width, height);
	Vec3 rgbSW = getPixelAt(img, pos.x - 1, pos.y + 1, width, height);
	Vec3 rgbSE = getPixelAt(img, pos.x + 1, pos.y + 1, width, height);
	Vec3 rgbM  = getPixelAt(img, pos.x, pos.y, width, height);
	Vec3 luma(0.299, 0.587, 0.114);
	float lumaNW = rgbNW.dot(luma);
	float lumaNE = rgbNE.dot(luma);
	float lumaSW = rgbSW.dot(luma);
	float lumaSE = rgbSE.dot(luma);
	float lumaM  = rgbM.dot(luma);
	float lumaMin = std::min(lumaM, std::min(std::min(lumaNW, lumaNE), std::min(lumaSW, lumaSE)));
	float lumaMax = std::max(lumaM, std::max(std::max(lumaNW, lumaNE), std::max(lumaSW, lumaSE)));
	Vec2 dir(-((lumaNW + lumaNE) - (lumaSW + lumaSE)), ((lumaNW + lumaSW) - (lumaNE + lumaSE)));
	float dirReduce = std::max((lumaNW + lumaNE + lumaSW + lumaSE) * (.25f * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);
	float rcpDirMin = 1. / (std::min(std::abs(dir.x), std::abs(dir.y)) + dirReduce);
	dir *= rcpDirMin;
	dir.clamp(-FXAA_SPAN_MAX, FXAA_SPAN_MAX);
	Vec3 a1 = getPixelAt(img, (int64_t)std::round(pos.x + dir.x * (1. / 3. - .5)), (int64_t)std::round(pos.y + dir.y * (1. / 3. - .5)), width, height);
	Vec3 a2 = getPixelAt(img, (int64_t)std::round(pos.x + dir.x * (2. / 3. - .5)), (int64_t)std::round(pos.y + dir.y * (2. / 3. - .5)), width, height);
	Vec3 rgbA(a1 + a2);
	rgbA /= 2;
	Vec3 rgbB(rgbA / 2);
	Vec3 b1 = getPixelAt(img, (int64_t)std::round(pos.x + dir.x * -.5), (int64_t)std::round(pos.y + dir.y * -.5), width, height);
	Vec3 b2 = getPixelAt(img, (int64_t)std::round(pos.x + dir.x *  .5), (int64_t)std::round(pos.y + dir.y *  .5), width, height);
	rgbB += (b1 + b2) * .25;
	float lumaB = rgbB.dot(luma);
	if ((lumaB < lumaMin) || (lumaB > lumaMax))
		return (rgbA);
	return (rgbB);
}

Vec3 *Fxaa::fxaa(Vec3 *img, uint64_t width, uint64_t height)
{
	Vec3 *newImg = new Vec3[width * height];
	for (uint64_t x = 0; x < width; ++x)
	{
		for (uint64_t y = 0; y < height; ++y)
		{
			Vec2 pos(x, y);
			newImg[x + y * width] = process(img, pos, width, height);
		}
	}
	return (newImg);
}
