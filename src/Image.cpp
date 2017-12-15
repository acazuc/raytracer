#include "Image.h"
#include "Debug.h"
#include <algorithm>
#include <cmath>

Image::Image(uint32_t width, uint32_t height)
: filtering(IMAGE_FILTERING_NEAREST)
, wrap(IMAGE_WRAP_REPEAT)
{
	this->data = new Vec4[width * height];
}

Image::Image()
: filtering(IMAGE_FILTERING_NEAREST)
, wrap(IMAGE_WRAP_REPEAT)
, height(0)
, width(0)
, data(NULL)
{
	//Empty
}

void Image::setData(uint64_t width, uint64_t height, uint8_t *data)
{
	this->width = width;
	this->height = height;
	this->data = new Vec4[this->width * this->height];
	for (uint64_t i = 0; i < this->width * this->height; ++i)
	{
		uint64_t idx = i * 4;
		this->data[i] = Vec4(data[idx] / 255., data[idx + 1] / 255., data[idx + 2] / 255., data[idx + 3] / 255.);
	}
}

Vec4 Image::getTexelAt(uint32_t x, uint32_t y)
{
	return (this->data[std::min(this->width - 1, std::max(0u, x)) + std::min(this->height - 1, std::max(0u, y)) * this->width]);
}

static Vec4 cubicInterpolate(Vec4 *values, float x)
{
	return (values[1] + (values[2] - values[0] + (values[0] * 2.f - values[1] * 5.f + values[2] * 4.f - values[3] + ((values[1] - values[2]) * 3.f + values[3] - values[0]) * x) * x) * x * .5f);
}

Vec4 Image::getDataAt(Vec2 uv)
{
	if (!this->data)
		return (0);
	switch (this->wrap)
	{
		case IMAGE_WRAP_CLAMP:
			uv.x = std::min(1.f, std::max(0.f, uv.x));
			uv.y = std::min(1.f, std::max(0.f, uv.y));
			break;
		case IMAGE_WRAP_MIRRORED_REPEAT:
			uv.x = fmod(uv.x, 2);
			if (uv.x < 0)
				uv.x += 2;
			uv.x = std::abs(uv.x - 1);
			uv.y = fmod(uv.y, 2);
			if (uv.y < 0)
				uv.y += 2;
			uv.y = std::abs(uv.y - 1);
			break;
		case IMAGE_WRAP_REPEAT:
		default:
			uv.x = std::fmod(uv.x, 1);
			if (uv.x < 0)
				++uv.x;
			uv.y = std::fmod(uv.y, 1);
			if (uv.y < 0)
				++uv.y;
	}
	switch (filtering)
	{
		case IMAGE_FILTERING_CUBIC:
			{
				uint32_t x0 = uv.x * this->width;
				uint32_t y0 = uv.y * this->height;
				float px = (uv.x - (float)x0 / this->width) * this->width;
				float py = (uv.y - (float)y0 / this->height) * this->height;
				Vec4 p[4];
				Vec4 p2[4];
				for (int32_t i = 0; i < 4; ++i)
				{
					p2[0] = getTexelAt(std::min(this->width, x0 - 1), std::min(this->height, y0 + i - 1));
					p2[1] = getTexelAt(std::min(this->width, x0 + 0), std::min(this->height, y0 + i - 1));
					p2[2] = getTexelAt(std::min(this->width, x0 + 1), std::min(this->height, y0 + i - 1));
					p2[3] = getTexelAt(std::min(this->width, x0 + 2), std::min(this->height, y0 + i - 1));
					p[i] = cubicInterpolate(p2, px);
				}
				return (cubicInterpolate(p, py));
			}
		case IMAGE_FILTERING_LINEAR:
			{
				uint32_t x0 = uv.x * this->width;
				uint32_t x1 = std::min(this->width, x0 + 1);
				uint32_t y0 = uv.y * this->height;
				uint32_t y1 = std::min(this->height, y0 + 1);
				float px = (uv.x - (float)x0 / this->width) * this->width;
				float py = (uv.y - (float)y0 / this->height) * this->height;
				Vec4 p00 = getTexelAt(x0, y0);
				Vec4 p01 = getTexelAt(x0, y1);
				Vec4 p10 = getTexelAt(x1, y0);
				Vec4 p11 = getTexelAt(x1, y1);
				Vec4 p0 = p00 * (1 - py) + p01 * py;
				Vec4 p1 = p10 * (1 - py) + p11 * py;
				return (p0 * (1 - px) + p1 * px);
			}
		case IMAGE_FILTERING_NEAREST:
		default:
			return (getTexelAt(uv.x * this->width, uv.y * this->height));
	}
}
