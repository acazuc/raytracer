#ifndef OBJECT_H
# define OBJECT_H

# include "Mat/Mat3.h"
# include "Vec/Vec4.h"
# include "Vec/Vec3.h"
# include "Vec/Vec2.h"
# include <string>

class Material;
class Image;
class Ray;

class Object
{

	public:
		Material *material;
		Mat3 invMat;
		Mat3 mat;
		Vec3 position;
		Object();
		virtual ~Object() {};
		void setMaterial(Material *material);
		void setPosition(Vec3 position);
		void setRotation(Vec3 rotation);
		virtual bool collide(Ray &ray, float &t) = 0;
		virtual Vec2 getUVAt(Ray &ray, Vec3 &pos) = 0;
		virtual Vec3 getNormAt(Ray &ray, Vec3 &pos) = 0;

};

#endif
