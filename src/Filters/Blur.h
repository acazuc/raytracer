#ifndef BLUR_H
# define BLUR_H

# include "./Filter.h"

class Blur : public Filter
{

	private:
		size_t radius;
		Vec4 process(Vec4 *src, size_t x, size_t y, size_t width, size_t height);

	public:
		Blur(size_t radius);
		void operator()(Vec4 *dst, Vec4 *src, float *zBuffer, size_t width, size_t height);

};

#endif
