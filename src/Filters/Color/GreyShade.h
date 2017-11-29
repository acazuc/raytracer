#ifndef GREY_SHADE_H
# define GREY_SHADE_H

# include "Vec3.h"
# include <cstdint>

class GreyShade
{

	public:
		static Vec3 *greyShade(Vec3 *img, uint64_t width, uint64_t height);

};

#endif
