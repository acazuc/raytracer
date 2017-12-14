#ifndef SEPIA_H
# define SEPIA_H

# include "Vec/Vec4.h"
# include <cstdint>

class Sepia
{

	public:
		static Vec4 *sepia(Vec4 *img, uint64_t width, uint64_t height);

};

#endif
