#include "DepthOfField.h"

DepthOfField::DepthOfField(float focal, float aperture)
: aperture(aperture)
, focal(focal)
{
}

Vec4 DepthOfField::operator()(Vec4 *colorBuffer, float *zBuffer, size_t x, size_t y, size_t width, size_t height)
{
	Vec4 org(colorBuffer[x + y * width]);
	Vec3 total(0);
	float count = 0;
	ssize_t radius = std::fabs(zBuffer[x + y * width] - this->focal) / this->aperture;
	for (ssize_t xx = -radius; xx <= radius; ++xx)
	{
		for (ssize_t yy = -radius; yy <= radius; ++yy)
		{
			float factor;
			if (radius == 0)
			{
				factor = 1;
			}
			else
			{
				float dx = float(xx) / radius;
				float dy = float(yy) / radius;
				factor = std::sqrt(dx * dx + dy * dy);
				if (factor > 1)
					factor = 1;
				factor = 1 - factor;
			}
			total += getPixelAt(colorBuffer, x + xx, y + yy, width, height).rgb() * factor;
			count += factor;
		}
	}
	return Vec4(clamp(total / count, 0.f, 1.f), org.a);
}
