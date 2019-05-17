#ifndef GLOW_H
# define GLOW_H

# include "Filter.h"

class Glow : public Filter
{

	private:
		float threshold;
		float intensity;
		size_t radius;

	public:
		Glow(size_t radius, float threshold, float intensity);
		Vec4 operator()(Vec4 *colorBuffer, float *zBuffer, size_t x, size_t y, size_t width, size_t height);

};

#endif
