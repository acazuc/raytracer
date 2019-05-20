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

bool Triangle::collide(Ray &ray, CollisionContext &collision)
{
	Vec3 rdir(this->invMat * ray.dir);
	Vec3 p(cross(rdir, this->e2));
	float det = dot(this->e1, p);
	if (det > -EPSILON && det < EPSILON)
		return false;
	det = 1. / det;
	Vec3 rpos(this->invMat * (ray.pos - this->position));
	Vec3 tt(rpos - this->p1);
	float u = dot(tt, p) * det;
	if (u < 0 || u > 1)
		return false;
	Vec3 q(cross(tt, this->e1));
	float v = dot(rdir, q) * det;
	if (v < 0 || u + v > 1)
		return false;
	float t = dot(this->e2, q) * det;
	if (t < EPSILON)
		return false;
	if (t > collision.t)
		return false;
	collision.tmpData[0] = u;
	collision.tmpData[1] = v;
	collision.object = this;
	collision.t = t;
	return true;
}

Vec2 Triangle::getUVAt(CollisionContext &collision)
{
	float u = collision.tmpData[0];
	float v = collision.tmpData[1];
	return u * this->UV2 + v * this->UV3 + (1 - u - v) * this->UV1;
}

Vec3 Triangle::getNormAt(CollisionContext &collision)
{
	float u = collision.tmpData[0];
	float v = collision.tmpData[1];
	Vec3 norm(u * this->norm2 + v * this->norm3 + (1 - u - v) * this->norm1);
	return normalize(this->mat * norm);
}

void Triangle::updateNorms()
{
	if (this->sets == TRIANGLE_NORMS)
		return;
	Vec3 norm(normalize(cross(this->e1, this->e2)));
	if (!(this->sets & TRIANGLE_NORM1))
		this->norm1 = norm;
	if (!(this->sets & TRIANGLE_NORM2))
		this->norm2 = norm;
	if (!(this->sets & TRIANGLE_NORM3))
		this->norm3 = norm;
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
	this->sets |= TRIANGLE_NORM3;
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
