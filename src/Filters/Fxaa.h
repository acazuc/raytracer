#ifndef FXAA_H
# define FXAA_H

# include "./Filter.h"

class Fxaa : public Filter
{

	private:
		Vec4 process(Vec4 *img, size_t x, size_t y, size_t width, size_t height);

	public:
		void operator()(Vec4 *dst, Vec4 *src, float *zBuffer, size_t width, size_t height);

};

#endif
