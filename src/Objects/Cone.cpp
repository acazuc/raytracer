#include "Cone.h"
#include "Quadratic.h"
#include "Consts.h"
#include <cmath>

Cone::Cone(float size)
: size(size)
{
	//Empty
}

bool Cone::collide(Ray &ray, float &t)
{
	Vec3 delta(this->unrotMat * (ray.pos - this->pos));
	Vec3 delta2(delta);
	delta2.y = -delta2.y;
	Vec3 rdir(this->unrotMat * ray.dir);
	Vec3 rdir2(rdir);
	rdir2.y = -rdir2.y;
	Quadratic quadratic;
	quadratic.a = dot(rdir2, rdir);
	quadratic.b = dot(rdir2, delta) * 2.f;
	quadratic.c = dot(delta2, delta) - 9;
	quadratic.solve();
	return (t = quadratic.getMinPosT()) >= EPSILON;
}

Vec2 Cone::getUVAt(Ray &ray, Vec3 &pos)
{
	(void)ray;
	Vec3 norm(normalize(this->unrotMat * (pos - this->pos)));
	norm.y = -norm.y;
	return Vec2(.5f + atan2(norm.z, norm.x) / (2 * M_PI), .5 + pos.y);
}

Vec3 Cone::getNormAt(Ray &ray, Vec3 &pos)
{
	(void)ray;
	Vec3 norm(this->unrotMat * (pos - this->pos));
	norm.y = -norm.y;
	return normalize(this->rotMat * norm);
}
