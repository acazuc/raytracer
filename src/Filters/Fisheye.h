#ifndef FISHEYE_H
# define FISHEYE_H

# include "Vec/Vec4.h"
# include <cstdint>

class Fisheye
{

	public:
		static Vec4 *fisheye(Vec4 *img, uint64_t width, uint64_t height);

};

#endif
