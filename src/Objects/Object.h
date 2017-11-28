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
		float specularFactor;
		float transparency;
		float reflection;
		float refraction;
		float specular;
		Object();
		virtual inline Vec3 *collide(Ray &ray) {(void)ray;return (nullptr);};
		virtual inline Vec3 getNormAt(Ray &ray, Vec3 &pos) {(void)ray;(void)pos;return (Vec3(0, 0, 1));};

};

#endif
