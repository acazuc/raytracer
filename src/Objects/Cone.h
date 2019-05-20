#ifndef CONE_H
# define CONE_H

# include "./Object.h"

class Cone : public Object
{

	private:
		float sizeSq;
		float size;

	public:
		Cone();
		bool collide(Ray &ray, CollisionContext &collision);
		Vec2 getUVAt(CollisionContext &collision);
		Vec3 getNormAt(CollisionContext &collision);
		void setSize(float size);
		inline float getSize() {return this->size;};

};

#endif
