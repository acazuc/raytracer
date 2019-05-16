#ifndef FILTER_H
# define FILTER_H

# include "Vec/Vec4.h"
# include <cstdint>

class Filter
{

	protected:
		Vec4 getPixelAt(Vec4 *img, size_t x, size_t y, size_t width, size_t height);
		float getZAt(float *zBuffer, size_t x, size_t y, size_t width, size_t height);

	public:
		virtual ~Filter() {};
		virtual void operator()(Vec4 *dst, Vec4 *src, float *zBuffer, size_t width, size_t height) = 0;

};

#endif
