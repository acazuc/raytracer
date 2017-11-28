#ifndef CONE_H
# define CONE_H

# include "./Object.h"

class Cone : public Object
{

	public:
		float size;
		Cone(float size);
		Vec3 *collide(Ray &ray);
		Vec3 getNormAt(Vec3 &pos);

};

#endif
