#ifndef GAMMA_H
# define GAMMA_H

# include "../Filter.h"

class Gamma : public Filter
{

	private:
		float gamma;
		Vec4 process(Vec4 *img, size_t x, size_t y, size_t width, size_t height);

	public:
		Gamma(float gamma);
		void operator()(Vec4 *dst, Vec4 *src, float *zBuffer, size_t width, size_t height);

};

#endif
