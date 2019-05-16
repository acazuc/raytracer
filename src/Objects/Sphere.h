#ifndef SPHERE_H
# define SPHERE_H

# include "./Object.h"

class Sphere : public Object
{

	private:
		float size;

	public:
		Sphere();
		bool collide(Ray &ray, float &t);
		Vec2 getUVAt(Ray &ray, Vec3 &pos);
		Vec3 getNormAt(Ray &ray, Vec3 &pos);
		void setSize(float size);
		inline float getSize() {return this->size;};

};

#endif
