#ifndef BLUR_H
# define BLUR_H

# include "Vec/Vec4.h"
# include <cstdint>

class Blur
{

	public:
		static Vec4 *blur(Vec4 *img, uint64_t offset, uint64_t width, uint64_t height);

};

#endif
