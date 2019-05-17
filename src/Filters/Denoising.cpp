#include "Denoising.h"

Denoising::Denoising(size_t radius, float threshold)
: radius(radius)
, threshold(threshold)
{
}

Vec4 Denoising::operator()(Vec4 *colorBuffer, float *zBuffer, size_t x, size_t y, size_t width, size_t height)
{
	Vec4 org(colorBuffer[x + y * width]);
	float lengthOrg = length(org);
	Vec4 total(org);
	size_t count = 1;
	for (ssize_t yy = -ssize_t(this->radius); yy <= ssize_t(this->radius); ++yy)
	{
		for (ssize_t xx = -ssize_t(this->radius); xx <= ssize_t(this->radius); ++xx)
		{
			if (yy == 0 && xx == 0)
				continue;
			Vec4 sample(getPixelAt(colorBuffer, x + xx, y + yy, width, height));
			if (dot(sample, org) / (length(sample) * lengthOrg) > this->threshold)
			{
				total += sample;
				count++;
			}
		}
	}
	return total / float(count);
	(void)zBuffer;
}
