#include "Image.h"
#include "Debug.h"
#include <algorithm>
#include <cmath>

Image::Image(size_t width, size_t height)
: filtering(IMAGE_FILTERING_NEAREST)
, wrap(IMAGE_WRAP_REPEAT)
, data(width * height)
{
}

Image::Image()
: filtering(IMAGE_FILTERING_NEAREST)
, wrap(IMAGE_WRAP_REPEAT)
, height(0)
, width(0)
{
}

void Image::setData(size_t width, size_t height, const uint8_t *data)
{
	this->width = width;
	this->height = height;
	this->data.resize(this->width * this->height);
	size_t idx = 0;
	for (size_t i = 0; i < this->data.size(); ++i)
	{
		this->data[i].r = data[idx++] / 255.;
		this->data[i].g = data[idx++] / 255.;
		this->data[i].b = data[idx++] / 255.;
		this->data[i].a = data[idx++] / 255.;
	}
}

Vec4 Image::getTexelAt(ssize_t x, ssize_t y)
{
	switch (this->wrap)
	{
		case IMAGE_WRAP_CLAMP:
			if (x >= ssize_t(this->width))
				x = this->width;
			else if (x < 0)
				x = 0;
			if (y >= ssize_t(this->height))
				y = this->height;
			else if (y < 0)
				y = 0;
			break;
		case IMAGE_WRAP_MIRRORED_REPEAT:
			x %= this->width * 2;
			if (x < 0)
				x += this->width * 2;
			x = std::abs(x - ssize_t(this->width));
			y %= this->height * 2;
			if (y < 0)
				y += this->height * 2;
			y = std::abs(y - ssize_t(this->height));
			break;
		case IMAGE_WRAP_REPEAT:
			x %= this->width;
			if (x < 0)
				x += this->width;
			y %= this->height;
			if (y < 0)
				y += this->height;
			break;
	}
	return this->data[x + y * this->width];
}

static Vec4 cubicInterpolate(Vec4 *values, float x)
{
	return values[1] + (values[2] - values[0] + (values[0] * 2.f - values[1] * 5.f + values[2] * 4.f - values[3] + ((values[1] - values[2]) * 3.f + values[3] - values[0]) * x) * x) * x * .5f;
}

Vec4 Image::getDataAt(Vec2 uv)
{
	if (!this->data.size())
		return 0;
	switch (filtering)
	{
		case IMAGE_FILTERING_CUBIC:
		{
			int32_t x0 = uv.x * this->width;
			int32_t y0 = uv.y * this->height;
			float px = (uv.x - float(x0) / this->width) * this->width;
			float py = (uv.y - float(y0) / this->height) * this->height;
			Vec4 p[4];
			Vec4 p2[4];
			for (int32_t i = 0; i < 4; ++i)
			{
				p2[0] = getTexelAt(x0 - 1, y0 + i - 1);
				p2[1] = getTexelAt(x0 + 0, y0 + i - 1);
				p2[2] = getTexelAt(x0 + 1, y0 + i - 1);
				p2[3] = getTexelAt(x0 + 2, y0 + i - 1);
				p[i] = cubicInterpolate(p2, px);
			}
			return cubicInterpolate(p, py);
		}
		case IMAGE_FILTERING_LINEAR:
		{
			int32_t x0 = uv.x * this->width;
			int32_t x1 = x0 + 1;
			int32_t y0 = uv.y * this->height;
			int32_t y1 = y0 + 1;
			float px = (uv.x - float(x0) / this->width) * this->width;
			float py = (uv.y - float(y0) / this->height) * this->height;
			Vec4 p00 = getTexelAt(x0, y0);
			Vec4 p01 = getTexelAt(x0, y1);
			Vec4 p10 = getTexelAt(x1, y0);
			Vec4 p11 = getTexelAt(x1, y1);
			Vec4 p0 = p00 * (1 - py) + p01 * py;
			Vec4 p1 = p10 * (1 - py) + p11 * py;
			return p0 * (1 - px) + p1 * px;
		}
		case IMAGE_FILTERING_NEAREST:
		default:
			return getTexelAt(uv.x * this->width, uv.y * this->height);
	}
}
