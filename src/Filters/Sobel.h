#ifndef SOBEL_H
# define SOBEL_H

# include "./Filter.h"

class Sobel : public Filter
{

	private:
		float threshold;

	public:
		Sobel(float threshold);
		Vec4 operator()(Vec4 *colorBuffer, float *zBuffer, size_t x, size_t y, size_t width, size_t height);

};

#endif
