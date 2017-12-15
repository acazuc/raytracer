#include "Triangle.h"
#include "Consts.h"
#include <cmath>

bool Triangle::collide(Ray &ray, float &t)
{
	Vec3 e1(this->pos2 - this->pos);
	Vec3 e2(this->pos3 - this->pos);
	Vec3 rdir(this->unrotMat * ray.dir);
	Vec3 p(ray.dir.cross(e2));
	float det = e1.dot(p);
	if (det == 0)
		return (false);
	det = 1 / det;
	Vec3 o(this->unrotMat * (ray.pos - this->pos));
	float u = o.dot(p) * det;
	if (u < EPSILON || u > 1 + EPSILON)
		return (false);
	Vec3 q(o.cross(e1));
	float v = ray.dir.dot(q) * det;
	if (v < EPSILON || v > 1 + EPSILON)
		return (false);
	return ((t = e2.dot(q) * det) > 0);
}

Vec2 Triangle::getUVAt(Ray &ray, Vec3 &pos)
{
	(void)ray;
	(void)pos;
	return (Vec2(0, 0));
}

Vec3 Triangle::getNormAt(Ray &ray, Vec3 &pos)
{
	(void)ray;
	(void)pos;
	Vec3 u(this->pos2 - this->pos);
	Vec3 v(this->pos3 - this->pos);
	Vec3 norm(this->rotMat * u.cross(v));
	norm.normalize();
	return (norm);
}
