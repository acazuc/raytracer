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
	delta.rotate(-this->rot);
	ray.dir.rotate(-this->rot);
	Quadratic quadratic;
	quadratic.a = ray.dir.dot(ray.dir);
	quadratic.b = 2 * ray.dir.dot(delta);
	quadratic.c = delta.dot(delta) - this->size;
	quadratic.solve();
	ray.dir.rotate(this->rot);
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
