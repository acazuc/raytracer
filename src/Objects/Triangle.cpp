#include "Triangle.h"
#include "Raytracer.h"
#include "Consts.h"
#include "Ray.h"
#include "Debug.h"
#include <cmath>

Triangle::Triangle()
: sets(0)
{
}

bool Triangle::collide(Ray &ray, float &t)
{
	Vec3 rdir(ray.dir);
	Vec3 p(cross(rdir, this->e2));
	float det = dot(this->e1, p);
	if (det == 0)
		return false;
	det = 1. / det;
	Vec3 rpos(ray.pos - this->position);
	Vec3 tt(rpos - this->p1);
	float u = dot(tt, p) * det;
	if (u < EPSILON || u >= 1 + EPSILON)
		return false;
	Vec3 q(cross(tt, this->e1));
	float v = dot(rdir, q) * det;
	if (v < EPSILON || u + v > 1 + EPSILON)
		return false;
	return (t = dot(this->e2, q) * det) >= EPSILON;
}

Vec2 Triangle::getUVAt(CollisionContext &collision)
{
	(void)collision;
	return Vec2(0, 0);
}

Vec3 Triangle::getNormAt(CollisionContext &collision)
{
	(void)collision;
	return normalize(this->mat * this->norm1);
}

void Triangle::updateNorms()
{
	if (this->sets == TRIANGLE_NORMS)
		return;
	Vec3 norm(cross(this->e1, this->e2));
	if (!(this->sets & TRIANGLE_NORM1))
		this->norm1 = norm;
	if (!(this->sets & TRIANGLE_NORM2))
		this->norm1 = norm;
	if (!(this->sets & TRIANGLE_NORM3))
		this->norm1 = norm;
}

void Triangle::setPoint1(Vec3 p1)
{
	this->p1 = p1;
	this->e1 = this->p2 - this->p1;
	this->e2 = this->p3 - this->p1;
	updateNorms();
}

void Triangle::setPoint2(Vec3 p2)
{
	this->p2 = p2;
	this->e1 = this->p2 - this->p1;
	updateNorms();
}

void Triangle::setPoint3(Vec3 p3)
{
	this->p3 = p3;
	this->e2 = this->p3 - this->p1;
	updateNorms();
}

void Triangle::setNorm1(Vec3 norm)
{
	this->sets |= TRIANGLE_NORM1;
	this->norm1 = norm;
}

void Triangle::setNorm2(Vec3 norm)
{
	this->sets |= TRIANGLE_NORM2;
	this->norm2 = norm;
}

void Triangle::setNorm3(Vec3 norm)
{
	this->sets |= TRIANGLE_NORM1;
	this->norm3 = norm;
}

void Triangle::setUV1(Vec2 UV)
{
	this->UV1 = UV;
}

void Triangle::setUV2(Vec2 UV)
{
	this->UV2 = UV;
}

void Triangle::setUV3(Vec2 UV)
{
	this->UV3 = UV;
}
