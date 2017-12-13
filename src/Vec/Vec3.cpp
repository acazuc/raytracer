#include "Vec3.h"
#include <cmath>

Vec3 Vec3::reflect(Vec3 vec)
{
	return (*this - vec * 2 * this->dot(vec));
}

Vec3 Vec3::cross(Vec3 vec)
{
	return (Vec3(this->y * vec.z - this->z * vec.y, this->z * vec.x - this->x * vec.z, this->x * vec.y - this->y * vec.x));
}

float Vec3::dot(Vec3 vec)
{
	return (this->x * vec.x + this->y * vec.y + this->z * vec.z);
}

float Vec3::angle(Vec3 vec)
{
	return (acos(dot(vec) / (length() * vec.length())));
}

float Vec3::length()
{
	return (sqrt(dot(*this)));
}

void Vec3::normalize()
{
	*this = *this / this->length();
}

void Vec3::rotate(Vec3 vec)
{
	rotateX(vec.x);
	rotateY(vec.y);
	rotateZ(vec.z);
}

void Vec3::unrotate(Vec3 vec)
{
	rotateZ(-vec.z);
	rotateY(-vec.y);
	rotateX(-vec.x);
}

void Vec3::rotateX(float angle)
{
	if (!angle)
		return;
	float tmpY = this->y;
	float tmpZ = this->z;
	this->y = tmpY * cos(angle) - tmpZ * sin(angle);
	this->z = tmpY * sin(angle) + tmpZ * cos(angle);
}

void Vec3::rotateY(float angle)
{
	if (!angle)
		return;
	float tmpX = this->x;
	float tmpZ = this->z;
	this->x = tmpZ * sin(angle) + tmpX * cos(angle);
	this->z = tmpZ * cos(angle) - tmpX * sin(angle);
}

void Vec3::rotateZ(float angle)
{
	if (!angle)
		return;
	float tmpX = this->x;
	float tmpY = this->y;
	this->x = tmpX * cos(angle) - tmpY * sin(angle);
	this->y = tmpX * sin(angle) + tmpY * cos(angle);
}

float &Vec3::operator [] (int idx)
{
	return (reinterpret_cast<float*>(this)[idx]);
}

Vec3 Vec3::operator - ()
{
	return (Vec3(-this->x, -this->y, -this->z));
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

Vec3 Vec3::operator += (float val)
{
	this->x += val;
	this->y += val;
	this->z += val;
	return (*this);
}

Vec3 Vec3::operator -= (float val)
{
	this->x -= val;
	this->y -= val;
	this->z -= val;
	return (*this);
}

Vec3 Vec3::operator *= (float val)
{
	this->x *= val;
	this->y *= val;
	this->z *= val;
	return (*this);
}

Vec3 Vec3::operator /= (float val)
{
	this->x /= val;
	this->y /= val;
	this->z /= val;
	return (*this);
}

Vec3 Vec3::operator += (Vec3 vec)
{
	this->x += vec.x;
	this->y += vec.y;
	this->z += vec.z;
	return (*this);
}

Vec3 Vec3::operator -= (Vec3 vec)
{
	this->x -= vec.x;
	this->y -= vec.y;
	this->z -= vec.z;
	return (*this);
}

Vec3 Vec3::operator *= (Vec3 vec)
{
	this->x *= vec.x;
	this->y *= vec.y;
	this->z *= vec.z;
	return (*this);
}

Vec3 Vec3::operator /= (Vec3 vec)
{
	this->x /= vec.x;
	this->y /= vec.y;
	this->z /= vec.z;
	return (*this);
}