#include "Sphere.h"
#include "Quadratic.h"
#include "Debug.h"

Sphere::Sphere(float size)
: size(size)
{
	//Empty
}

Vec3 *Sphere::collide(Ray &ray)
{
	Vec3 delta(ray.pos - this->pos);
	delta.unrotate(this->rot);
	Vec3 rdir(ray.dir);
	rdir.unrotate(this->rot);
	Quadratic quadratic;
	quadratic.a = rdir.dot(rdir);
	quadratic.b = 2 * rdir.dot(delta);
	quadratic.c = delta.dot(delta) - this->size;
	quadratic.solve();
	float t = quadratic.getMinPosT();
	if (t < 0)
		return (nullptr);
	return (new Vec3(ray.pos + ray.dir * t));
}

Vec3 Sphere::getNormAt(Ray &ray, Vec3 &pos)
{
	(void)ray;
	Vec3 vec(pos - this->pos);
	vec.normalize();
	return (vec);
}
