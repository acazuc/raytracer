#ifndef SPHERE_H
# define SPHERE_H

# include "./Object.h"

class Sphere : public Object
{

	private:
		float sizeSq;
		float size;

	public:
		Sphere();
		bool collide(Ray &ray, CollisionContext &collision);
		Vec2 getUVAt(CollisionContext &collision);
		Vec3 getNormAt(CollisionContext &collision);
		void setSize(float size);
		inline float getSize() {return this->size;};

};

#endif
