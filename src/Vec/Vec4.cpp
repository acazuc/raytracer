#include "Vec4.h"
#include <cmath>

float Vec4::dot(Vec4 vec)
{
	return (this->x * vec.x + this->y * vec.y + this->z * vec.z + this->w * vec.w);
}

float Vec4::angle(Vec4 vec)
{
	return (acos(dot(vec) / (length() * vec.length())));
}

float Vec4::length()
{
	return (sqrt(dot(*this)));
}

void Vec4::normalize()
{
	*this = *this / this->length();
}

Vec3 Vec4::xyz()
{
	return (Vec3(this->x, this->y, this->z));
}

Vec4 Vec4::operator + (float val)
{
	return (Vec4(this->x + val, this->y + val, this->z + val, this->w + val));
}

Vec4 Vec4::operator - (float val)
{
	return (Vec4(this->x - val, this->y - val, this->z - val, this->w - val));
}

Vec4 Vec4::operator * (float val)
{
	return (Vec4(this->x * val, this->y * val, this->z * val, this->w * val));
}

Vec4 Vec4::operator / (float val)
{
	return (Vec4(this->x / val, this->y / val, this->z / val, this->w / val));
}

Vec4 Vec4::operator + (Vec4 vec)
{
	return (Vec4(this->x + vec.x, this->y + vec.y, this->z + vec.z, this->w + vec.w));
}

Vec4 Vec4::operator - (Vec4 vec)
{
	return (Vec4(this->x - vec.x, this->y - vec.y, this->z - vec.z, this->w - vec.w));
}

Vec4 Vec4::operator * (Vec4 vec)
{
	return (Vec4(this->x * vec.x, this->y * vec.y, this->z * vec.z, this->w * vec.w));
}

Vec4 Vec4::operator / (Vec4 vec)
{
	return (Vec4(this->x / vec.x, this->y / vec.y, this->z / vec.z, this->w / vec.w));
}

Vec4 Vec4::operator += (float val)
{
	this->x += val;
	this->y += val;
	this->z += val;
	this->w += val;
	return (*this);
}

Vec4 Vec4::operator -= (float val)
{
	this->x -= val;
	this->y -= val;
	this->z -= val;
	this->w -= val;
	return (*this);
}

Vec4 Vec4::operator *= (float val)
{
	this->x *= val;
	this->y *= val;
	this->z *= val;
	this->w *= val;
	return (*this);
}

Vec4 Vec4::operator /= (float val)
{
	this->x /= val;
	this->y /= val;
	this->z /= val;
	this->w /= val;
	return (*this);
}

Vec4 Vec4::operator += (Vec4 vec)
{
	this->x += vec.x;
	this->y += vec.y;
	this->z += vec.z;
	this->w += vec.w;
	return (*this);
}

Vec4 Vec4::operator -= (Vec4 vec)
{
	this->x -= vec.x;
	this->y -= vec.y;
	this->z -= vec.z;
	this->w -= vec.w;
	return (*this);
}

Vec4 Vec4::operator *= (Vec4 vec)
{
	this->x *= vec.x;
	this->y *= vec.y;
	this->z *= vec.z;
	this->w *= vec.w;
	return (*this);
}

Vec4 Vec4::operator /= (Vec4 vec)
{
	this->x /= vec.x;
	this->y /= vec.y;
	this->z /= vec.z;
	this->w /= vec.w;
	return (*this);
}
