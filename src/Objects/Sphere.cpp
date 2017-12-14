#include "Sphere.h"
#include "Quadratic.h"
#include "Debug.h"
#include <cmath>

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

Vec2 Sphere::getUVAt(Ray &ray, Vec3 &pos)
{
	(void)ray;
	Vec3 norm(pos - this->pos);
	norm.unrotate(this->rot);
	norm.normalize();
	Vec2 uv(.5 + atan2(norm.z, norm.x) / (2 * M_PI), .5 - asin(norm.y) / M_PI);
	return (uv);
}

Vec3 Sphere::getNormAt(Ray &ray, Vec3 &pos)
{
	(void)ray;
	Vec3 vec(pos - this->pos);
	if (this->bumpTexture)
	{
		Vec4 bump = this->bumpTexture->getDataAt(getUVAt(ray, pos));
		Vec3 T(-vec.y, vec.x);
		T.normalize();
		//Vec3 P(vec.x, 0, vec.z);
		//P.normalize();
		Vec3 B(vec.cross(T));
		vec = T * bump.r + B * bump.g + vec * bump.b;
	}
	vec.normalize();
	return (vec);
}
