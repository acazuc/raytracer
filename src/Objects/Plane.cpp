#include "Plane.h"
#include "Debug.h"
#include <cmath>

bool Plane::collide(Ray &ray, Vec3 &pos)
{
	Vec3 norm(0, 1, 0);
	norm = this->rotMat * norm;
	Vec3 delta(this->pos - ray.pos);
	float dot = norm.dot(ray.dir);
	if (dot == 0)
		return (false);
	float t = norm.dot(delta) / dot;
	if (t < 0)
		return (false);
	pos = ray.pos + ray.dir * t;
	return (true);
}

Vec2 Plane::getUVAt(Ray &ray, Vec3 &pos)
{
	(void)ray;
	Vec3 rel(pos - this->pos);
	rel = this->unrotMat * rel;
	Vec2 uv(rel.x / 4, rel.z / 4);
	return (uv);
}

Vec3 Plane::getNormAt(Ray &ray, Vec3 &pos)
{
	(void)pos;
	Vec3 vec(0, 1, 0);
	if (this->N_map)
	{
		Vec4 bump = this->N_map->getDataAt(getUVAt(ray, pos));
		Vec3 tmp = (bump.rgb() - .5) * 2;
		vec = Vec3(-tmp.r, tmp.b, tmp.g);
		vec.normalize();
	}
	vec = this->rotMat * vec;
	if (vec.dot(ray.dir) / (vec.length() * ray.dir.length()) > 0)
		vec = -vec;
	return (vec);
}
