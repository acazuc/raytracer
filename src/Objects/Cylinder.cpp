#include "Cylinder.h"
#include "Quadratic.h"
#include "Debug.h"
#include <cmath>

Cylinder::Cylinder(float size)
: size(size)
{
	//Empty
}

bool Cylinder::collide(Ray &ray, Vec3 &pos)
{
	Vec3 delta(ray.pos - this->pos);
	delta = this->unrotMat * delta;
	delta.y = 0;
	Vec3 rdir(ray.dir);
	rdir = this->unrotMat * rdir;
	rdir.y = 0;
	Quadratic quadratic;
	quadratic.a = rdir.dot(rdir);
	quadratic.b = 2 * rdir.dot(delta);
	quadratic.c = delta.dot(delta) - this->size / 2;
	quadratic.solve();
	float t = quadratic.getMinPosT();
	if (t < 0)
		return (false);
	pos = ray.pos + ray.dir * t;
	return (true);
}

Vec2 Cylinder::getUVAt(Ray &ray, Vec3 &pos)
{
	(void)ray;
	Vec3 norm(pos - this->pos);
	norm = this->unrotMat * norm;
	norm.y = 0;
	norm.normalize();
	Vec2 uv(.5 + atan2(norm.z, norm.x) / (2 * M_PI), .5 + pos.y / 4);
	return (uv);
}

Vec3 Cylinder::getNormAt(Ray &ray, Vec3 &pos)
{
	(void)ray;
	Vec3 vec(pos - this->pos);
	vec = this->unrotMat * vec;
	vec.y = 0;
	if (this->N_map)
	{
		Vec4 bump = this->N_map->getDataAt(getUVAt(ray, pos));
		bump = (bump - .5) * 2;
		Vec3 T(0, 1, 0);
		Vec3 B(-vec.cross(T));
		vec = B * bump.r + T * bump.g + vec * -bump.b;
	}
	vec.normalize();
	vec = this->rotMat * vec;
	if (vec.dot(ray.dir) / (vec.length() * ray.dir.length()) > 0)
		vec = -vec;
	return (vec);
}
