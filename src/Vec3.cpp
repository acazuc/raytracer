#include "Vec3.h"
#include <cmath>

float Vec3::dot(Vec3 vec)
{
	return (this->x * vec.x + this->y * vec.y + this->z * vec.z);
}

float Vec3::length()
{
	return (sqrt(dot(*this)));
}

Vec3 Vec3::operator + (float val)
{
	return (Vec3(this->x + val, this->y + val, this->z + val));
}

Vec3 Vec3::operator - (float val)
{
	return (Vec3(this->x - val, this->y - val, this->z - val));
}

Vec3 Vec3::operator * (float val)
{
	return (Vec3(this->x * val, this->y * val, this->z * val));
}

Vec3 Vec3::operator / (float val)
{
	return (Vec3(this->x / val, this->y / val, this->z / val));
}

Vec3 Vec3::operator + (Vec3 vec)
{
	return (Vec3(this->x + vec.x, this->y + vec.y, this->z + vec.z));
}

Vec3 Vec3::operator - (Vec3 vec)
{
	return (Vec3(this->x - vec.x, this->y - vec.y, this->z - vec.z));
}

Vec3 Vec3::operator * (Vec3 vec)
{
	return (Vec3(this->x * vec.x, this->y * vec.y, this->z * vec.z));
}

Vec3 Vec3::operator / (Vec3 vec)
{
	return (Vec3(this->x / vec.x, this->y / vec.y, this->z / vec.z));
}
