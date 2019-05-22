#ifndef PLANE_H
# define PLANE_H

# include "./Object.h"

class Plane : public Object
{

	private:
		Vec3 normal;

	public:
		Plane(Vec3 normal);
		Plane();
		void collide(Ray &ray, CollisionContext &collision);
		Vec2 getUVAt(CollisionContext &collision);
		Vec3 getNormAt(CollisionContext &collision);

};

#endif
