#include "Plane.h"
#include "Raytracer.h"
#include "Material.h"
#include "Texture.h"
#include "Consts.h"
#include "Ray.h"
#include <cmath>

Plane::Plane(Vec3 normal)
: normal(normal)
{
}

Plane::Plane()
: normal(0, 1, 0)
{
}

void Plane::collide(Ray &ray, CollisionContext &collision)
{
	Vec3 norm(this->mat * this->normal);
	float d = dot(norm, ray.dir);
	if (d == 0)
		return;
	Vec3 delta(this->position - ray.pos);
	float t = dot(norm, delta) / d;
	if (t < EPSILON)
		return;
	if (t > collision.t)
		return;
	collision.object = this;
	collision.t = t;
	return;
}

Vec2 Plane::getUVAt(CollisionContext &collision)
{
	Vec3 rel(this->invMat * (collision.pos - this->position));
	return Vec2(rel.x / 4, rel.z / 4);
}

Vec3 Plane::getNormAt(CollisionContext &collision)
{
	Vec3 norm(this->normal);
	if (this->material->normalTexture)
	{
		Vec4 bump(this->material->normalTexture->getDataAt(collision.UV));
		Vec3 tmp((bump.rgb() - .5f) * 2.f);
		norm = Vec3(-tmp.r, tmp.b, tmp.g);
	}
	return normalize(this->mat * norm);
}
