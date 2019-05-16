#include "PunctualLight.h"

Vec3 PunctualLight::getDirectionFrom(Vec3 &pos)
{
	Vec3 vec(this->position - pos);
	return vec;
}

void PunctualLight::setPosition(Vec3 position)
{
	this->position = position;
}
