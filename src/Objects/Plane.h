#ifndef PLANE_H
# define PLANE_H

# include "./Object.h"

class Plane : public Object
{

	public:
		bool collide(Ray &ray, float &t);
		Vec2 getUVAt(CollisionContext &collision);
		Vec3 getNormAt(CollisionContext &collision);

};

#endif
