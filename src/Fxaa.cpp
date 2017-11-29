#include "Fxaa.h"
#include <algorithm>

#define FXAA_REDUCE_MIN (1.f / 128.f)
#define FXAA_SPAN_MAX 8.f
#define FXAA_REDUCE_MUL (1.f / 8.f)

Vec3 Fxaa::process(Vec3 *img, Vec2 &pos, uint64_t width, uint64_t height)
{
	(void)height;
	Vec3 rgbNW = img[(int64_t)(pos.x - 1 + (pos.y - 1) * width)];
	Vec3 rgbNE = img[(int64_t)(pos.x + 1 + (pos.y - 1) * width)];
	Vec3 rgbSW = img[(int64_t)(pos.x - 1 + (pos.y + 1) * width)];
	Vec3 rgbSE = img[(int64_t)(pos.x + 1 + (pos.y + 1) * width)];
	Vec3 rgbM  = img[(int64_t)(pos.x + pos.y * width)];
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
	dir.x = std::min(FXAA_SPAN_MAX, std::max(-FXAA_SPAN_MAX, dir.x * rcpDirMin));
	dir.y = std::min(FXAA_SPAN_MAX, std::max(-FXAA_SPAN_MAX, dir.y * rcpDirMin));
	Vec3 a1 = img[(int64_t)(pos.x + dir.x * (1. / 3. - .5) + (int64_t)(pos.y + dir.y * (1. / 3. - .5)) * width)];
	Vec3 a2 = img[(int64_t)(pos.x + dir.x * (2. / 3. - .5) + (int64_t)(pos.y + dir.y * (2. / 3. - .5)) * width)];
	Vec3 rgbA(a1 + a2);
	rgbA /= 2;
	Vec3 rgbB(rgbA / 2);
	Vec3 b1 = img[(int64_t)(pos.x + dir.x * -.5 + (int64_t)(pos.y + dir.y * -.5) * width)];
	Vec3 b2 = img[(int64_t)(pos.x + dir.x *  .5 + (int64_t)(pos.y + dir.y *  .5) * width)];
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
