#ifndef SPHERE_H
# define SPHERE_H

# include "./Object.h"

class Sphere : public Object
{

	public:
		float size;
		Sphere(float size);
		Vec3 *collide(Ray &ray);
		Vec3 getNormAt(Ray &ray, Vec3 &pos);

};

#endif
