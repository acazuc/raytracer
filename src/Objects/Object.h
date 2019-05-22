#ifndef OBJECT_H
# define OBJECT_H

# include "Mat/Mat3.h"
# include "Vec/Vec4.h"
# include "Vec/Vec3.h"
# include "Vec/Vec2.h"
# include <string>

class CollisionContext;
class Material;
class Image;
class Ray;

class Object
{

	private:
		void calcMatrix();

	public:
		Material *material;
		Vec3 position;
		Vec3 rotation;
		Vec3 scale;
		Mat3 invMat;
		Mat3 mat;
		Object();
		virtual ~Object() {};
		void setMaterial(Material *material);
		void setPosition(Vec3 position);
		void setRotation(Vec3 rotation);
		void setScale(Vec3 scale);
		virtual void collide(Ray &ray, CollisionContext &collision) = 0;
		virtual Vec2 getUVAt(CollisionContext &collision) = 0;
		virtual Vec3 getNormAt(CollisionContext &collision) = 0;

};

#endif
