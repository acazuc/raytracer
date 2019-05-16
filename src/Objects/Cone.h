#ifndef CONE_H
# define CONE_H

# include "./Object.h"

class Cone : public Object
{

	private:
		float size;

	public:
		Cone();
		bool collide(Ray &ray, float &t);
		Vec2 getUVAt(Ray &ray, Vec3 &pos);
		Vec3 getNormAt(Ray &ray, Vec3 &pos);
		void setSize(float size);
		inline float getSize() {return this->size;};

};

#endif
