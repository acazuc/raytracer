#include "PonctualLight.h"

Vec3 PonctualLight::getDirectionFrom(Vec3 &pos)
{
	Vec3 vec(this->pos - pos);
	vec.normalize();
	return (vec);
}
