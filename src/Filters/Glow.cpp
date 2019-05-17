#include "Glow.h"

Glow::Glow(size_t radius, float threshold, float intensity)
: threshold(threshold)
, intensity(intensity)
, radius(radius)
{
}

Vec4 Glow::operator()(Vec4 *colorBuffer, float *zBuffer, size_t x, size_t y, size_t width, size_t height)
{
	Vec3 total(0);
	float count(0);
	for (ssize_t xx = -this->radius; xx <= ssize_t(this->radius); ++xx)
	{
		for (ssize_t yy = -this->radius; yy <= ssize_t(this->radius); ++yy)
		{
			float dx = float(xx) / this->radius;
			float dy = float(yy) / this->radius;
			float factor = std::sqrt(dx * dx + dy * dy);
			if (factor > 1)
				factor = 1;
			factor = 1 - factor;
			total += clamp((getPixelAt(colorBuffer, ssize_t(x) + xx, ssize_t(y) + yy, width, height).rgb() - threshold) / (1 - threshold), 0.f, 1.f) * factor;
			count += factor;
		}
	}
	Vec4 org(colorBuffer[x + y * width]);
	return Vec4(clamp(org.rgb() + total / count * this->intensity, 0.f, 1.f), org.a);
	(void)zBuffer;
}
