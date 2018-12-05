#include "Vec3.h"
#include <cmath>

void Vec3::rotateX(float angle)
{
	if (!angle)
		return;
	float tmpY = this->y;
	float tmpZ = this->z;
	float ca = cos(angle);
	float sa = sin(angle);
	this->y = tmpY * ca - tmpZ * sa;
	this->z = tmpY * sa + tmpZ * ca;
}

void Vec3::rotateY(float angle)
{
	if (!angle)
		return;
	float tmpX = this->x;
	float tmpZ = this->z;
	float ca = cos(angle);
	float sa = sin(angle);
	this->x = tmpZ * sa + tmpX * ca;
	this->z = tmpZ * ca - tmpX * sa;
}

void Vec3::rotateZ(float angle)
{
	if (!angle)
		return;
	float tmpX = this->x;
	float tmpY = this->y;
	float ca = cos(angle);
	float sa = sin(angle);
	this->x = tmpX * ca - tmpY * sa;
	this->y = tmpX * sa + tmpY * ca;
}
