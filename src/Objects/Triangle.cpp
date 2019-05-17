#include "Triangle.h"
#include "Consts.h"
#include "Ray.h"
#include <cmath>

bool Triangle::collide(Ray &ray, float &t)
{
	Vec3 e1(this->pos2 - this->position);
	Vec3 e2(this->pos3 - this->position);
	Vec3 rdir(this->invMat * ray.dir);
	Vec3 p(cross(ray.dir, e2));
	float det = dot(e1, p);
	if (det == 0)
		return false;
	det = 1 / det;
	Vec3 o(this->invMat * (ray.pos - this->position));
	float u = dot(o, p) * det;
	if (u < EPSILON || u > 1 + EPSILON)
		return false;
	Vec3 q(cross(o, e1));
	float v = dot(ray.dir, q) * det;
	if (v < EPSILON || v > 1 + EPSILON)
		return false;
	return (t = dot(e2, q) * det) > EPSILON;
}

Vec2 Triangle::getUVAt(Ray &ray, Vec3 &pos)
{
	(void)ray;
	(void)pos;
	return Vec2(0, 0);
}

Vec3 Triangle::getNormAt(Ray &ray, Vec3 &pos)
{
	(void)ray;
	(void)pos;
	Vec3 u(this->pos2 - this->position);
	Vec3 v(this->pos3 - this->position);
	return normalize(this->mat * cross(u, v));
}
