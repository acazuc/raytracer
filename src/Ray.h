#ifndef RAY_H
# define RAY_H

# include "Vec/Vec3.h"

class Ray
{

	public:
		Vec3 pos;
		Vec3 dir;
		float Ni;
		Ray() {};
		Ray(Vec3 pos, Vec3 dir) : pos(pos), dir(dir) {};

};

#endif
