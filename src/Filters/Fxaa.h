#ifndef FXAA_H
# define FXAA_H

# include "Vec3.h"
# include <cstdint>

class Fxaa
{

	private:
		static Vec3 getPixelAt(Vec3 *img, uint64_t x, uint64_t y, uint64_t width, uint64_t height);
		static Vec3 process(Vec3 *img, Vec2 &pos, uint64_t width, uint64_t height);

	public:
		static Vec3 *fxaa(Vec3 *img, uint64_t width, uint64_t height);

};

#endif
