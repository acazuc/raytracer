#include "Cylinder.h"
#include "Quadratic.h"
#include "Debug.h"

Cylinder::Cylinder(float size)
: size(size)
{
	//Empty
}

Vec3 *Cylinder::collide(Ray &ray)
{
	Vec3 delta(ray.pos - this->pos);
	delta.y = 0;
	Vec3 rdir(ray.dir);
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

Vec3 Cylinder::getNormAt(Vec3 &pos)
{
	Vec3 vec(pos - this->pos);
	vec.y = 0;
	vec.normalize();
	return (vec);
}
