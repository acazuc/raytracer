#include "Blur.h"

Blur::Blur(float radius)
: radius(radius)
{
}

Vec4 Blur::operator()(Vec4 *colorBuffer, float *zBuffer, size_t x, size_t y, size_t width, size_t height)
{
	Vec4 total(0);
	float count = 0;
	for (ssize_t xx = -ssize_t(this->radius); xx <= ssize_t(this->radius); ++xx)
	{
		for (ssize_t yy = -ssize_t(this->radius); yy <= ssize_t(this->radius); ++yy)
		{
			float dx = float(xx) / this->radius;
			float dy = float(yy) / this->radius;
			float factor = std::sqrt(dx * dx + dy * dy);
			if (factor > 1)
				factor = 1;
			factor = 1 - factor;
			total += getPixelAt(colorBuffer, ssize_t(x) + xx, ssize_t(y) + yy, width, height) * factor;
			count += factor;
		}
	}
	return total / count;
	(void)zBuffer;
}
