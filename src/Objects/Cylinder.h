#ifndef CYLINDER_H
# define CYLINDER_H

# include "./Object.h"

class Cylinder : public Object
{

	private:
		float sizeSq;
		float size;

	public:
		Cylinder();
		bool collide(Ray &ray, CollisionContext &collision);
		Vec2 getUVAt(CollisionContext &collision);
		Vec3 getNormAt(CollisionContext &collision);
		void setSize(float size);
		inline float getSize() {return this->size;};

};

#endif
