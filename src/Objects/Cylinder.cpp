#include "Cylinder.h"
#include "Quadratic.h"
#include "Debug.h"
#include <cmath>

Cylinder::Cylinder(float size)
: size(size)
{
	//Empty
}

Vec3 *Cylinder::collide(Ray &ray)
{
	Vec3 delta(ray.pos - this->pos);
	delta.unrotate(this->rot);
	delta.y = 0;
	Vec3 rdir(ray.dir);
	rdir.unrotate(this->rot);
	rdir.y = 0;
	Quadratic quadratic;
	quadratic.a = ray.dir.dot(rdir);
	quadratic.b = 2 * rdir.dot(delta);
	quadratic.c = delta.dot(delta) - this->size;
	quadratic.solve();
	float t = quadratic.getMinPosT();
	if (t < 0)
		return (nullptr);
	return (new Vec3(ray.pos + ray.dir * t));
}

Vec2 Cylinder::getUVAt(Ray &ray, Vec3 &pos)
{
	(void)ray;
	Vec3 norm(pos - this->pos);
	norm.unrotate(this->rot);
	norm.y = 0;
	norm.normalize();
	Vec2 uv(std::asin(norm.x) / M_PI + .5, pos.y);
	return (uv);
}

Vec3 Cylinder::getNormAt(Ray &ray, Vec3 &pos)
{
	(void)ray;
	Vec3 vec(pos - this->pos);
	vec.unrotate(this->rot);
	vec.y = 0;
	vec.rotate(this->rot);
	vec.normalize();
	return (vec);
}
