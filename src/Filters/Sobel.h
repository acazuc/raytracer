#ifndef SOBEL_H
# define SOBEL_H

# include "Vec/Vec4.h"
# include <cstdint>

class Sobel
{

	public:
		static Vec4 *sobel(Vec4 *img, float *zbuffer, uint64_t width, uint64_t height);

};

#endif
