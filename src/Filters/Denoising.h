#ifndef DENOISING_H
# define DENOISING_H

# include "./Filter.h"

class Denoising : public Filter
{

	private:
		size_t radius;
		float threshold;

	public:
		Denoising(size_t radius, float threshold);
		Vec4 operator()(Vec4 *colorBuffer, float *zBuffer, size_t x, size_t y, size_t width, size_t height);

};

#endif
