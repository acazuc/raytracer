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
	quadratic.a = rdir.dot(rdir);
	quadratic.b = 2 * rdir.dot(delta);
	quadratic.c = delta.dot(delta) - this->size / 2;
	quadratic.solve();
	return ((t = quadratic.getMinPosT() >= EPSILON));
}

Vec2 Cylinder::getUVAt(Ray &ray, Vec3 &pos)
{
	(void)ray;
	Vec3 norm(this->unrotMat * (pos - this->pos));
	norm.y = 0;
	norm.normalize();
	Vec2 uv(.5 + atan2(norm.z, norm.x) / (2 * M_PI), .5 + pos.y / 4);
	return (uv);
}

Vec3 Cylinder::getNormAt(Ray &ray, Vec3 &pos)
{
	Vec3 norm(this->unrotMat * (pos - this->pos));
	norm.y = 0;
	if (this->N_map)
	{
		Vec4 bump = this->N_map->getDataAt(getUVAt(ray, pos));
		bump = (bump - .5) * 2;
		Vec3 T(0, 1, 0);
		Vec3 B(-norm.cross(T));
		norm = B * bump.r + T * bump.g + norm * -bump.b;
	}
	norm = this->rotMat * norm;
	norm.normalize();
	return (norm);
}
