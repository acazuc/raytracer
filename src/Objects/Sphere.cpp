#include "Sphere.h"
#include "Raytracer.h"
#include "Quadratic.h"
#include "Material.h"
#include "Consts.h"
#include "Image.h"
#include "Ray.h"
#include <cmath>

Sphere::Sphere()
: sizeSq(0)
, size(0)
{
}

bool Sphere::collide(Ray &ray, float &t)
{
	Vec3 delta(this->invMat * (ray.pos - this->position));
	Vec3 rdir(this->invMat * ray.dir);
	Quadratic quadratic;
	quadratic.a = dot(rdir, rdir);
	quadratic.b = dot(rdir, delta) * 2.f;
	quadratic.c = dot(delta, delta) - this->sizeSq;
	if (!quadratic.solve())
		return false;
	return (t = quadratic.getMinPosT()) >= EPSILON;
}

Vec2 Sphere::getUVAt(CollisionContext &collision)
{
	Vec3 norm(normalize(this->invMat * (collision.pos - this->position)));
	return Vec2(.5f + atan2(norm.z, norm.x) / (2 * M_PI), .5f - asin(norm.y) / M_PI);
}

Vec3 Sphere::getNormAt(CollisionContext &collision)
{
	Vec3 norm(collision.pos - this->position);
	if (this->material->normalTexture)
	{
		Vec4 bump(this->material->normalTexture->getDataAt(collision.UV));
		Vec3 tmp((bump.rgb() - .5f) * 2.f);
		float val = atan2(norm.z, norm.x);
		Vec3 T(cos(val + M_PI / 2), 0, sin(val + M_PI / 2));
		Vec3 B(cross(norm, T));
		norm = T * tmp.r - B * tmp.g + norm * tmp.b;
	}
	return normalize(norm);
}

void Sphere::setSize(float size)
{
	this->size = size;
	this->sizeSq = this->size * this->size;
}
