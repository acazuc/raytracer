#include "Sphere.h"
#include "Quadratic.h"
#include "Consts.h"
#include <cmath>

Sphere::Sphere(float size)
: size(size)
{
	//Empty
}

bool Sphere::collide(Ray &ray, float &t)
{
	Vec3 delta(this->unrotMat * (ray.pos - this->pos));
	Vec3 rdir(this->unrotMat * ray.dir);
	Quadratic quadratic;
	quadratic.a = rdir.dot(rdir);
	quadratic.b = 2 * rdir.dot(delta);
	quadratic.c = delta.dot(delta) - this->size / 2;
	quadratic.solve();
	return ((t = quadratic.getMinPosT()) >= EPSILON);
}

Vec2 Sphere::getUVAt(Ray &ray, Vec3 &pos)
{
	(void)ray;
	Vec3 norm(this->unrotMat * (pos - this->pos));
	norm.normalize();
	Vec2 uv(.5 + atan2(norm.z, norm.x) / (2 * M_PI), .5 - asin(norm.y) / M_PI);
	return (uv);
}

Vec3 Sphere::getNormAt(Ray &ray, Vec3 &pos)
{
	(void)ray;
	Vec3 vec(pos - this->pos);
	if (this->N_map)
	{
		Vec4 bump = this->N_map->getDataAt(getUVAt(ray, pos));
		bump = (bump - .5) * 2;
		Vec3 T(0, 1, 0);
		Vec3 B(-vec.cross(T));
		vec = T * bump.r + B * bump.g + vec * -bump.b;
	}
	vec.normalize();
	if (vec.dot(ray.dir) > 0)
		vec = -vec;
	return (vec);
}
