#include "Triangle.h"
#include <cmath>

#define EPSILON 0.000001

Vec3 *Triangle::collide(Ray &ray)
{
	Vec3 e1(this->pos2 - this->pos);
	Vec3 e2(this->pos3 - this->pos);
	Vec3 rdir(ray.dir);
	ray.dir.unrotate(this->rot);
	Vec3 p(ray.dir.cross(e2));
	float det = e1.dot(p);
	if (det == 0)
		return (nullptr);
	det = 1 / det;
	Vec3 o(ray.pos - this->pos);
	o.unrotate(this->rot);
	float u = o.dot(p) * det;
	if (u < EPSILON || u > 1 + EPSILON)
		return (nullptr);
	Vec3 q(o.cross(e1));
	float v = ray.dir.dot(q) * det;
	if (v < EPSILON || v > 1 + EPSILON)
		return (nullptr);
	float t = e2.dot(q) * det;
	if (t <= 0)
		return (nullptr);
	return (new Vec3(ray.pos + ray.dir * t));
}

Vec2 Triangle::getUVAt(Ray &ray, Vec3 &pos)
{
	(void)ray;
	(void)pos;
	return (Vec2(0, 0));
}

Vec3 Triangle::getNormAt(Ray &ray, Vec3 &pos)
{
	(void)pos;
	Vec3 u(this->pos2 - this->pos);
	Vec3 v(this->pos3 - this->pos);
	Vec3 vec(u.cross(v));
	vec.rotate(this->rot);
	if (vec.angle(ray.dir) <= M_PI / 2)
		vec = -vec;
	return (vec);
}
