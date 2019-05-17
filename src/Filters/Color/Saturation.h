#ifndef SATURATION_H
# define SATURATION_H

# include "../Filter.h"

class Saturation : public Filter
{

	private:
		float saturation;

	public:
		Saturation(float saturation);
		Vec4 operator()(Vec4 *colorBuffer, float *zBuffer, size_t x, size_t y, size_t width, size_t height);

};

#endif
