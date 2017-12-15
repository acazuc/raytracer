#ifndef FOG_H
# define FOG_H

# include "Vec/Vec4.h"
# include <cstdint>

enum FogType
{
	FOG_LINEAR,
	FOG_EXP,
	FOG_EXP2
};

class Fog
{

	public:
		static Vec4 *fog(Vec4 *img, float *zbuffer, uint64_t width, uint64_t height, Vec4 color, float start, float density, enum FogType type);

};

#endif
