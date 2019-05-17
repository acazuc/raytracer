#ifndef FOG_H
# define FOG_H

# include "./Filter.h"

enum FogType
{
	FOG_LINEAR,
	FOG_EXP,
	FOG_EXP2
};

class Fog : public Filter
{

	private:
		enum FogType type;
		Vec3 color;
		float v1;
		float v2;

	public:
		Fog(enum FogType type, Vec3 color, float v1, float v2);
		Vec4 operator()(Vec4 *colorBuffer, float *zBuffer, size_t x, size_t y, size_t width, size_t height);

};

#endif
