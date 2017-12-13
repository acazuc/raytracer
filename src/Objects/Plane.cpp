#include "Plane.h"
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
	(void)pos;
	return (Vec2(0, 0));
}

Vec3 Plane::getNormAt(Ray &ray, Vec3 &pos)
{
	(void)pos;
	Vec3 vec(0, 1, 0);
	vec.rotate(this->rot);
	if (vec.angle(ray.dir) <= M_PI / 2)
		vec = -vec;
	return (vec);
}
