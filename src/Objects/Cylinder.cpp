#include "Cylinder.h"
#include "Quadratic.h"
#include "Consts.h"
#include <cmath>

Cylinder::Cylinder(float size)
: size(size)
{
	//Empty
}

bool Cylinder::collide(Ray &ray, float &t)
{
	Vec3 delta(this->unrotMat * (ray.pos - this->pos));
	delta.y = 0;
	Vec3 rdir(this->unrotMat * ray.dir);
	rdir.y = 0;
	Quadratic quadratic;
	quadratic.a = dot(rdir, rdir);
	quadratic.b = dot(rdir, delta) * 2.f;
	quadratic.c = dot(delta, delta) - this->size * this->size;
	quadratic.solve();
	return (t = quadratic.getMinPosT()) >= EPSILON;
}

Vec2 Cylinder::getUVAt(Ray &ray, Vec3 &pos)
{
	(void)ray;
	Vec3 norm(this->unrotMat * (pos - this->pos));
	norm.y = 0;
	norm = normalize(norm);
	return Vec2(.5f + atan2(norm.z, norm.x) / (2 * M_PI), .5f + pos.y / 4);
}

Vec3 Cylinder::getNormAt(Ray &ray, Vec3 &pos)
{
	//Vec3 norm(normalize(pos - this->pos));
	Vec3 norm(this->unrotMat * (pos - this->pos));
	norm.y = 0;
	/*if (this->N_map)
	{
		Vec4 bump = this->N_map->getDataAt(getUVAt(ray, pos));
		bump = (bump - .5) * 2;
		Vec3 T(0, 1, 0);
		Vec3 B(-norm.cross(T));
		norm = B * bump.r + T * bump.g + norm * bump.b;
	}*/
	(void)ray;
	return normalize(this->rotMat * norm);
}
