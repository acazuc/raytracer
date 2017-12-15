#include "Plane.h"
#include "Consts.h"
#include <cmath>


bool Plane::collide(Ray &ray, float &t)
{
	Vec3 norm(this->rotMat * Vec3(0, 1, 0));
	Vec3 delta(this->pos - ray.pos);
	float dot = norm.dot(ray.dir);
	if (dot == 0)
		return (false);
	return ((t = norm.dot(delta) / dot) > EPSILON);
}

Vec2 Plane::getUVAt(Ray &ray, Vec3 &pos)
{
	(void)ray;
	Vec3 rel(this->unrotMat * (pos - this->pos));
	Vec2 uv(rel.x / 4, rel.z / 4);
	return (uv);
}

Vec3 Plane::getNormAt(Ray &ray, Vec3 &pos)
{
	Vec3 norm(0, 1, 0);
	if (this->N_map)
	{
		Vec4 bump = this->N_map->getDataAt(getUVAt(ray, pos));
		Vec3 tmp = (bump.rgb() - .5) * 2;
		norm = Vec3(-tmp.r, tmp.b, tmp.g);
		norm.normalize();
	}
	norm = this->rotMat * norm;
	return (norm);
}
