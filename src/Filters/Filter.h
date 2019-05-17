#ifndef FILTER_H
# define FILTER_H

# include "Vec/Vec4.h"
# include <cstdint>

class Filter
{

	protected:
		Vec4 getPixelAt(Vec4 *img, size_t x, size_t y, size_t width, size_t height);
		float getZAt(float *zBuffer, size_t x, size_t y, size_t width, size_t height);
		Vec3 rgbToHsv(Vec3 rgb);
		Vec3 hsvToRgb(Vec3 hsv);

	public:
		virtual ~Filter() {};
		virtual Vec4 operator()(Vec4 *colorBuffer, float *zBuffer, size_t x, size_t y, size_t width, size_t height) = 0;

};

#endif
