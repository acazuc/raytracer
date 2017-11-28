#ifndef OBJECT_H
# define OBJECT_H

# include "Vec4.h"
# include "Vec3.h"
# include "Ray.h"

class Object
{

	public:
		Vec4 color;
		Vec3 pos;
		Vec3 rot;
		float transparency;
		float reflection;
		float refraction;
		virtual inline Vec3 *collide(Ray &ray) {(void)ray;return (nullptr);};
		virtual inline Vec3 *getNormalAt(Vec3 &pos) {(void)pos;return (nullptr);};

};

#endif
