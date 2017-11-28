#ifndef PLANE_H
# define PLANE_H

# include "./Object.h"

class Plane : public Object
{

	public:
		Vec3 *collide(Ray &ray);
		Vec3 getNormAt(Ray &ray, Vec3 &pos);

};

#endif
