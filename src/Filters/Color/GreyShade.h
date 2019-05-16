#ifndef GREY_SHADE_H
# define GREY_SHADE_H

# include "../Filter.h"

class GreyShade : public Filter
{

	public:
		void operator()(Vec4 *dst, Vec4 *src, float *zBuffer, size_t width, size_t height);

};

#endif
