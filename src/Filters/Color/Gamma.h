#ifndef GAMMA_H
# define GAMMA_H

# include "../Filter.h"

class Gamma : public Filter
{

	private:
		float gamma;

	public:
		Gamma(float gamma);
		Vec4 operator()(Vec4 *colorBuffer, float *zBuffer, size_t x, size_t y, size_t width, size_t height);

};

#endif
