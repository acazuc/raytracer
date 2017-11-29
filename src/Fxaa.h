#ifndef FXAA_H
# define FXAA_H

# include "Vec3.h"
# include "Vec2.h"
# include <cstdint>

class Fxaa
{

	private:
		static Vec3 process(Vec3 *img, Vec2 &posPos, uint64_t width, uint64_t height);

	public:
		static Vec3 *fxaa(Vec3 *img, uint64_t width, uint64_t height);

};

#endif
