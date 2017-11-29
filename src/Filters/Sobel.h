#ifndef SOBEL_H
# define SOBEL_H

# include "Vec3.h"
# include <cstdint>

class Sobel
{

	private:
		static Vec3 getPixelAt(Vec3 *img, uint64_t x, uint64_t y, uint64_t width, uint64_t height);
		static float getZIndexAt(float *zBuffer, uint64_t x, uint64_t y, uint64_t width, uint64_t height);
		static Vec3 process(Vec3 *img, float *zBuffer, Vec2 &pos, uint64_t width, uint64_t height);

	public:
		static Vec3 *sobel(Vec3 *img, float *zBuffer, uint64_t width, uint64_t height);

};

#endif
