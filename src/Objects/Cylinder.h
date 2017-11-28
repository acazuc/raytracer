#ifndef CYLINDER_H
# define CYLINDER_H

# include "./Object.h"

class Cylinder : public Object
{

	public:
		float size;
		Cylinder(float size);
		Vec3 *collide(Ray &ray);
		Vec3 getNormAt(Ray &ray, Vec3 &pos);

};

#endif
