#ifndef CEL_H
# define CEL_H

# include "Vec3.h"
# include <cstdint>

class Cel
{

	private:
		static Vec3 getPixelAt(Vec3 *img, uint64_t x, uint64_t y, uint64_t width, uint64_t height);

	public:
		static Vec3 *cel(Vec3 *img, uint8_t cels, uint64_t width, uint64_t height);

};

#endif
