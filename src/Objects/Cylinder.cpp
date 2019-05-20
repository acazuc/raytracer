#include "Cylinder.h"
#include "Raytracer.h"
#include "Quadratic.h"
#include "Material.h"
#include "Consts.h"
#include "Image.h"
#include "Ray.h"
#include <cmath>

Cylinder::Cylinder()
: sizeSq(0)
, size(0)
{
}

bool Cylinder::collide(Ray &ray, float &t)
{
	Vec3 delta(this->invMat * (ray.pos - this->position));
	delta.y = 0;
	Vec3 rdir(this->invMat * ray.dir);
	rdir.y = 0;
	Quadratic quadratic;
	quadratic.a = dot(rdir, rdir);
	quadratic.b = dot(rdir, delta) * 2.f;
	quadratic.c = dot(delta, delta) - this->sizeSq;
	if (!quadratic.solve())
		return false;
	return (t = quadratic.getMinPosT()) >= EPSILON;
}

Vec2 Cylinder::getUVAt(CollisionContext &collision)
{
	Vec3 norm(this->invMat * (collision.pos - this->position));
	float oldY = norm.y;
	norm.y = 0;
	//norm = normalize(norm);
	return Vec2(.5f + atan2(norm.z, norm.x) / (2 * M_PI), .5f + oldY / M_PI);
}

Vec3 Cylinder::getNormAt(CollisionContext &collision)
{
	Vec3 norm(this->invMat * (collision.pos - this->position));
	norm.y = 0;
	if (this->material->normalTexture)
	{
		Vec4 bump(this->material->normalTexture->getDataAt(collision.UV));
		Vec3 tmp((bump.rgb() - .5f) * 2.f);
		float val = atan2(norm.z, norm.x);
		Vec3 T(cos(val + M_PI / 2), 0, sin(val + M_PI / 2));
		Vec3 B(cross(norm, T));
		norm = T * tmp.r + B * tmp.g + norm * tmp.b;
	}
	return normalize(this->mat * norm);
}

void Cylinder::setSize(float size)
{
	this->size = size;
	this->sizeSq = this->size * this->size;
}
