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
	delta.unrotate(this->rot);
	Vec3 rdir(ray.dir);
	rdir.unrotate(this->rot);
	Quadratic quadratic;
	quadratic.a = rdir.x * rdir.x - rdir.y * rdir.y + rdir.z * rdir.z;
	quadratic.b = 2 * (rdir.x * delta.x - rdir.y * delta.y + rdir.z * delta.z);
	quadratic.c = delta.x * delta.x - delta.y * delta.y + delta.z * delta.z - this->size;
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
	vec.unrotate(this->rot);
	vec.y = -vec.y;
	vec.rotate(this->rot);
	vec.normalize();
	return (vec);
}
