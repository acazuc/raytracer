#include "DirectionalLight.h"

Vec3 DirectionalLight::getDirectionFrom(Vec3 &pos)
{
	(void)pos;
	Vec3 vec = -this->direction;
	return vec;
}

void DirectionalLight::setDirection(Vec3 direction)
{
	this->direction = direction;
}
