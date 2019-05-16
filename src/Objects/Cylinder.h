#ifndef CYLINDER_H
# define CYLINDER_H

# include "./Object.h"

class Cylinder : public Object
{

	private:
		float size;

	public:
		Cylinder();
		bool collide(Ray &ray, float &t);
		Vec2 getUVAt(Ray &ray, Vec3 &pos);
		Vec3 getNormAt(Ray &ray, Vec3 &pos);
		void setSize(float size);
		inline float getSize() {return this->size;};

};

#endif
