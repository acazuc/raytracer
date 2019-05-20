#ifndef TRIANGLE_H
# define TRIANGLE_H

# include "./Object.h"

enum TriangleSets
{
	TRIANGLE_NORM1 = 0x1,
	TRIANGLE_NORM2 = 0x2,
	TRIANGLE_NORM3 = 0x4,
	TRIANGLE_NORMS = 0x7
};

class Triangle : public Object
{

	private:
		void updateNorms();
		void updateBT();

	public:
		uint8_t sets;
		Vec3 norm1;
		Vec3 norm2;
		Vec3 norm3;
		Vec3 norm;
		Vec2 UVd1;
		Vec2 UVd2;
		Vec2 UV1;
		Vec2 UV2;
		Vec2 UV3;
		Vec3 e1;
		Vec3 e2;
		Vec3 p1;
		Vec3 p2;
		Vec3 p3;
		Triangle();
		bool collide(Ray &ray, CollisionContext &collision);
		Vec2 getUVAt(CollisionContext &collision);
		Vec3 getNormAt(CollisionContext &collision);
		void setPoint1(Vec3 p1);
		void setPoint2(Vec3 p2);
		void setPoint3(Vec3 p3);
		void setNorm1(Vec3 norm);
		void setNorm2(Vec3 norm);
		void setNorm3(Vec3 norm);
		void setUV1(Vec2 UV);
		void setUV2(Vec2 UV);
		void setUV3(Vec2 UV);

};

#endif
