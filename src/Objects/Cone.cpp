#include "Cone.h"
#include "Quadratic.h"
#include "Debug.h"

Cone::Cone(float size)
: size(size)
{
	//Empty
}

Vec3 *Cone::collide(Ray &ray)
{
	Vec3 delta(ray.pos - this->pos);
	delta.rotate(-this->rot);
	delta.y = -delta.y;
	Vec3 rdir(ray.dir);
	rdir.rotate(-this->rot);
	rdir.y = -rdir.y;
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

Vec3 Cone::getNormAt(Ray &ray, Vec3 &pos)
{
	(void)ray;
	Vec3 vec(pos - this->pos);
	vec.rotate(-this->rot);
	vec.y = -vec.y;
	vec.rotate(this->rot);
	vec.normalize();
	return (vec);
}
