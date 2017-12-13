#ifndef BLUR_H
# define BLUR_H

# include "Vec/Vec3.h"
# include <cstdint>

class Blur
{

	private:
		static Vec3 getPixelAt(Vec3 *img, uint64_t x, uint64_t y, uint64_t width, uint64_t height);
		static Vec3 process(Vec3 *img, uint64_t offset, uint64_t x, uint64_t y, uint64_t width, uint64_t height);

	public:
		static Vec3 *blur(Vec3 *img, uint64_t offset, uint64_t width, uint64_t height);

};

#endif
