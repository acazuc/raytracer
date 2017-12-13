#include "Image.h"
#include <algorithm>
#include <cmath>

Image::Image(uint32_t width, uint32_t height)
: filtering(IMAGE_FILTERING_NEAREST)
, wrap(IMAGE_WRAP_REPEAT)
{
	this->data = new uint8_t[width * height * 4];
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

Vec4 Image::getDataAt(Vec2 uv)
{
	if (!this->data)
		return (0);
	if (this->wrap == IMAGE_WRAP_CLAMP)
	{
		uv.x = std::min(1.f, std::max(0.f, uv.x));
		uv.y = std::min(1.f, std::max(0.f, uv.y));
	}
	else if (this->wrap == IMAGE_WRAP_REPEAT)
	{
		uv.x = std::fmod(uv.x, 1);
		if (uv.x < 0)
			++uv.x;
		uv.y = std::fmod(uv.y, 1);
		if (uv.y < 0)
			++uv.y;
	}
	else if (this->wrap == IMAGE_WRAP_MIRRORED_REPEAT)
	{
		uv.x = fmod(uv.x, 2);
		if (uv.x < 0)
			uv.x += 2;
		uv.x = std::abs(uv.x - 1);
		uv.y = fmod(uv.y, 2);
		if (uv.y < 0)
			uv.y += 2;
		uv.y = std::abs(uv.y - 1);
	}
	uint32_t idx = ((uint32_t)(uv.x * this->width) + (uint32_t)(uv.y * this->height) * this->width) * 4;
	uint8_t r = this->data[idx + 0];
	uint8_t g = this->data[idx + 1];
	uint8_t b = this->data[idx + 2];
	uint8_t a = this->data[idx + 3];
	return (Vec4(r / 255., g / 255., b / 255., a / 255.));
}
