#include "Cone.h"
#include "Raytracer.h"
#include "Quadratic.h"
#include "Material.h"
#include "Texture.h"
#include "Consts.h"
#include "Ray.h"
#include <cmath>

Cone::Cone(float size)
: sizeSq(size * size)
, size(size)
{
}

Cone::Cone()
: sizeSq(0)
, size(0)
{
}

void Cone::collide(Ray &ray, CollisionContext &collision)
{
	Vec3 delta(this->invMat * (ray.pos - this->position));
	Vec3 delta2(delta);
	delta2.y = -delta2.y;
	Vec3 rdir(this->invMat * ray.dir);
	Vec3 rdir2(rdir);
	rdir2.y = -rdir2.y;
	Quadratic quadratic;
	quadratic.a = dot(rdir2, rdir);
	quadratic.b = dot(rdir2, delta) * 2.f;
	quadratic.c = dot(delta2, delta) - this->sizeSq;
	if (!quadratic.solve())
		return;
	float t = quadratic.getMinPosT();
	if (t < EPSILON)
		return;
	if (t > collision.t)
		return;
	collision.object = this;
	collision.t = t;
}

Vec2 Cone::getUVAt(CollisionContext &collision)
{
	Vec3 norm(this->invMat * (collision.pos - this->position));
	float oldY = norm.y;
	norm.y = 0;
	return Vec2(.5f + atan2(norm.z, norm.x) / (2 * M_PI), oldY / M_PI);
}

Vec3 Cone::getNormAt(CollisionContext &collision)
{
	Vec3 norm(this->invMat * (collision.pos - this->position));
	norm.y = -norm.y;
	if (this->material->normalTexture)
	{
		Vec4 bump(this->material->normalTexture->getDataAt(collision.UV));
		Vec3 tmp((bump.rgb() - .5f) * 2.f);
		float val = atan2(norm.z, norm.x);
		Vec3 T(cos(val + M_PI / 2), 0, sin(val + M_PI / 2));
		Vec3 B(cross(norm, T));
		norm = T * tmp.r + B * tmp.g + norm * tmp.b;
	}
	return normalize(this->mat * norm);
}

void Cone::setSize(float size)
{
	this->size = size;
	this->sizeSq = this->size * this->size;
}
