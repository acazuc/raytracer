#ifndef FISHEYE_H
# define FISHEYE_H

# include "Vec/Vec3.h"
# include <cstdint>

class Fisheye
{

	private:
		static Vec3 getPixelAt(Vec3 *img, uint64_t x, uint64_t y, uint64_t width, uint64_t height);

	public:
		static Vec3 *fisheye(Vec3 *img, uint64_t width, uint64_t height);

};

#endif
