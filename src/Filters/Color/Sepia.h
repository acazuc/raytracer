#ifndef SEPIA_H
# define SEPIA_H

# include "Vec/Vec3.h"
# include <cstdint>

class Sepia
{

	public:
		static Vec3 *sepia(Vec3 *img, uint64_t width, uint64_t height);

};

#endif
