#include "Plane.h"
#include "Material.h"
#include "Consts.h"
#include "Image.h"
#include "Ray.h"
#include <cmath>

bool Plane::collide(Ray &ray, float &t)
{
	Vec3 norm(this->mat * Vec3(0, 1, 0));
	float d = dot(norm, ray.dir);
	if (d == 0)
		return false;
	Vec3 delta(this->position - ray.pos);
	return (t = dot(norm, delta) / d) >= EPSILON;
}

Vec2 Plane::getUVAt(Ray &ray, Vec3 &pos)
{
	(void)ray;
	Vec3 rel(this->invMat * (pos - this->position));
	return Vec2(rel.x / 4, rel.z / 4);
}

Vec3 Plane::getNormAt(Ray &ray, Vec3 &pos)
{
	Vec3 norm(0, 1, 0);
	if (this->material->normalTexture)
	{
		Vec4 bump = this->material->normalTexture->getDataAt(getUVAt(ray, pos));
		Vec3 tmp = (bump.rgb() - .5f) * 2.f;
		norm = normalize(Vec3(-tmp.r, tmp.b, tmp.g));
	}
	return this->mat * norm;
}
