#ifndef BLUR_H
# define BLUR_H

# include "./Filter.h"

class Blur : public Filter
{

	private:
		float radius;

	public:
		Blur(float radius);
		Vec4 operator()(Vec4 *colorBuffer, float *zBuffer, size_t x, size_t y, size_t width, size_t height);

};

#endif
