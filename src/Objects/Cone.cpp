#include "Cone.h"
#include "Quadratic.h"
#include "Debug.h"
#include <cmath>

Cone::Cone(float size)
: size(size)
{
	//Empty
}

bool Cone::collide(Ray &ray, Vec3 &pos)
{
	Vec3 delta(ray.pos - this->pos);
	delta = this->unrotMat * delta;
	Vec3 delta2(delta);
	delta2.y = -delta2.y;
	Vec3 rdir(ray.dir);
	rdir = this->unrotMat * rdir;
	Vec3 rdir2(rdir);
	rdir2.y = -rdir2.y;
	Quadratic quadratic;
	quadratic.a = ray.dir.dot(rdir2);
	quadratic.b = 2 * rdir2.dot(delta);
	quadratic.c = delta.dot(delta2) - 9;
	quadratic.solve();
	float t = quadratic.getMinPosT();
	if (t < 0)
		return (false);
	pos = ray.pos + ray.dir * t;
	return (true);
}

Vec2 Cone::getUVAt(Ray &ray, Vec3 &pos)
{
	(void)ray;
	Vec3 norm(pos - this->pos);
	norm = this->unrotMat * norm;
	norm.y = -norm.y;
	norm.normalize();
	Vec2 uv(.5 + atan2(norm.z, norm.x) / (2 * M_PI), .5 + pos.y);
	return (uv);
}

Vec3 Cone::getNormAt(Ray &ray, Vec3 &pos)
{
	(void)ray;
	Vec3 vec(pos - this->pos);
	vec = this->unrotMat * vec;
	vec.y = -vec.y;
	vec = this->rotMat * vec;
	/*if (this->bumpTexture)
	{
		Vec4 bump = this->bumpTexture->getDataAt(getUVAt(ray, pos));
		Vec3 tmp = (bump.rgb() - .5) * M_PI;
		tmp.z = -tmp.x;
		tmp.x = -tmp.y;
		tmp.y = tmp.z;
		tmp.z = 0;
		vec.rotate(tmp);
	}*/
	vec.normalize();
	if (vec.dot(ray.dir) / (vec.length() * ray.dir.length()) > 0)
		vec = -vec;
	return (vec);
}
