#ifndef TRIANGLE_H
# define TRIANGLE_H

# include "./Object.h"

class Triangle : public Object
{

	public:
		Vec3 pos2;
		Vec3 pos3;
		Vec3 *collide(Ray &ray);
		Vec2 getUVAt(Ray &ray, Vec3 &pos);
		Vec3 getNormAt(Ray &ray, Vec3 &pos);

};

#endif
