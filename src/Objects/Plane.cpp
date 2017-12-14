#include "Plane.h"
#include "Debug.h"
#include <cmath>

Vec3 *Plane::collide(Ray &ray)
{
	Vec3 norm(0, 1, 0);
	norm.rotate(this->rot);
	Vec3 delta(this->pos - ray.pos);
	float dot = norm.dot(ray.dir);
	if (dot == 0)
		return (nullptr);
	float t = norm.dot(delta) / dot;
	if (t < 0)
		return (nullptr);
	return (new Vec3(ray.pos + ray.dir * t));
}

Vec2 Plane::getUVAt(Ray &ray, Vec3 &pos)
{
	(void)ray;
	Vec3 rel(pos - this->pos);
	rel.unrotate(this->rot);
	Vec2 uv(rel.x, rel.z);
	return (uv);
}

Vec3 Plane::getNormAt(Ray &ray, Vec3 &pos)
{
	(void)pos;
	Vec3 vec(0, 1, 0);
	if (this->bumpTexture)
	{
		Vec4 bump = this->bumpTexture->getDataAt(getUVAt(ray, pos));
		Vec3 tmp = (bump.rgb() - .5) * M_PI;
		vec.rotate(Vec3(tmp.g, 0, tmp.r));
	}
	vec.rotate(this->rot);
	if (vec.angle(ray.dir) <= M_PI / 2)
		vec = -vec;
	return (vec);
}
