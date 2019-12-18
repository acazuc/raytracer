#include "Texture.h"
#include <algorithm>
#include <cmath>

Texture::Texture(size_t width, size_t height)
: filtering(TEXTURE_FILTERING_NEAREST)
, wrapS(TEXTURE_WRAP_REPEAT)
, wrapT(TEXTURE_WRAP_REPEAT)
, data(width * height)
{
}

Texture::Texture()
: filtering(TEXTURE_FILTERING_NEAREST)
, wrapS(TEXTURE_WRAP_REPEAT)
, wrapT(TEXTURE_WRAP_REPEAT)
, height(0)
, width(0)
{
}

void Texture::setData(size_t width, size_t height, const uint8_t *data)
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

Vec4 Texture::getTexelAt(ssize_t x, ssize_t y)
{
	switch (this->wrapS)
	{
		case TEXTURE_WRAP_CLAMP:
		{
			if (x >= ssize_t(this->width))
				x = this->width - 1;
			else if (x < 0)
				x = 0;
			break;
		}
		case TEXTURE_WRAP_MIRRORED_REPEAT:
		{
			ssize_t width2 = this->width * 2;
			x %= width2;
			if (x < 0)
				x += width2;
			if (x >= ssize_t(this->width))
				x = this->width - 1 - (x - ssize_t(this->width));
			break;
		}
		default:
		case TEXTURE_WRAP_REPEAT:
		{
			x %= ssize_t(this->width);
			if (x < 0)
				x += ssize_t(this->width);
			break;
		}
	}
	switch (this->wrapT)
	{
		case TEXTURE_WRAP_CLAMP:
		{
			if (y >= ssize_t(this->height))
				y = this->height - 1;
			else if (y < 0)
				y = 0;
			break;
		}
		case TEXTURE_WRAP_MIRRORED_REPEAT:
		{
			ssize_t height2 = this->height * 2;
			y %= height2;
			if (y < 0)
				y += height2;
			if (y >= ssize_t(this->height))
				y = this->height - 1 - (y - ssize_t(this->height));
			break;
		}
		default:
		case TEXTURE_WRAP_REPEAT:
		{
			y %= ssize_t(this->height);
			if (y < 0)
				y += ssize_t(this->height);
			break;
		}
	}
	return this->data[x + y * this->width];
}

static Vec4 cubicInterpolate(Vec4 *values, float x)
{
	Vec4 v0(values[0]);
	Vec4 v1(values[1]);
	Vec4 v2(values[2]);
	Vec4 v3(values[3]);
	return v1 + (v2 - v0 + (v0 * 2.f - v1 * 5.f + v2 * 4.f - v3 + ((v1 - v2) * 3.f + v3 - v0) * x) * x) * x * .5f;
}

Vec4 Texture::getDataAt(Vec2 uv)
{
	if (!this->data.size())
		return 0;
	switch (filtering)
	{
		case TEXTURE_FILTERING_CUBIC:
		{
			ssize_t x0 = std::floor(uv.x * this->width);
			ssize_t y0 = std::floor(uv.y * this->height);
			float px = (uv.x - float(x0) / this->width) * this->width;
			if (px < 0)
				px++;
			float py = (uv.y - float(y0) / this->height) * this->height;
			if (py < 0)
				py++;
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
		case TEXTURE_FILTERING_LINEAR:
		{
			ssize_t x0 = std::floor(uv.x * this->width);
			ssize_t x1 = x0 + 1;
			ssize_t y0 = std::floor(uv.y * this->height);
			ssize_t y1 = y0 + 1;
			float px = (uv.x - float(x0) / this->width) * this->width;
			if (px < 0)
				px++;
			float py = (uv.y - float(y0) / this->height) * this->height;
			if (py < 0)
				py++;
			Vec4 p00(getTexelAt(x0, y0));
			Vec4 p01(getTexelAt(x0, y1));
			Vec4 p10(getTexelAt(x1, y0));
			Vec4 p11(getTexelAt(x1, y1));
			Vec4 p0(p00 + (p01 - p00) * py);
			Vec4 p1(p10 + (p11 - p10) * py);
			return p0 + (p1 - p0) * px;
		}
		case TEXTURE_FILTERING_NEAREST:
		default:
			return getTexelAt(uv.x * this->width, uv.y * this->height);
	}
}

void Texture::setFiltering(enum TextureFilteringMode filtering)
{
	this->filtering = filtering;
}

void Texture::setWrapS(enum TextureWrapMode wrap)
{
	this->wrapS = wrap;
}

void Texture::setWrapT(enum TextureWrapMode wrap)
{
	this->wrapT = wrap;
}

void Texture::setWrap(enum TextureWrapMode wrap)
{
	setWrapS(wrap);
	setWrapT(wrap);
}
