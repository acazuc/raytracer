#ifndef SPHERE_H
# define SPHERE_H

# include "./Object.h"

class Sphere : public Object
{

	public:
		float size;
		Sphere(float size);
		Vec3 *collide(Ray &ray);

};

#endif
