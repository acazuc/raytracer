#ifndef GREY_SHADE_H
# define GREY_SHADE_H

# include "Vec/Vec4.h"
# include <cstdint>

class GreyShade
{

	public:
		static Vec4 *greyShade(Vec4 *img, uint64_t width, uint64_t height);

};

#endif
