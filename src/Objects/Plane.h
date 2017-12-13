#ifndef PLANE_H
# define PLANE_H

# include "./Object.h"

class Plane : public Object
{

	public:
		Vec3 *collide(Ray &ray);
		Vec2 getUVAt(Ray &ray, Vec3 &pos);
		Vec3 getNormAt(Ray &ray, Vec3 &pos);

};

#endif
