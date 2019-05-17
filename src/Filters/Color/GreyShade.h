#ifndef GREY_SHADE_H
# define GREY_SHADE_H

# include "../Filter.h"

class GreyShade : public Filter
{

	public:
		Vec4 operator()(Vec4 *colorBuffer, float *zBuffer, size_t x, size_t y, size_t width, size_t height);

};

#endif
