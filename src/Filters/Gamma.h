#ifndef GAMMA_H
# define GAMMA_H

# include "Vec/Vec3.h"
# include <cstdint>

class Gamma
{

	private:
		static Vec3 getPixelAt(Vec3 *img, uint64_t x, uint64_t y, uint64_t width, uint64_t height);
		static Vec3 process(Vec3 *img, Vec2 &pos, uint64_t width, uint64_t height, float gamma);

	public:
		static Vec3 *gamma(Vec3 *img, uint64_t width, uint64_t height, float gamma);

};

#endif
