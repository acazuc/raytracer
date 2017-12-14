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

Vec3 Vec4::rgb()
{
	return (Vec3(this->r, this->g, this->b));
}

Vec2 Vec4::xy()
{
	return (Vec2(this->x, this->y));
}

Vec2 Vec4::zw()
{
	return (Vec2(this->z, this->w));
}

float &Vec4::operator [] (int idx)
{
	return (reinterpret_cast<float*>(this)[idx]);
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
	return (*this = *this + val);
}

Vec4 Vec4::operator -= (float val)
{
	return (*this = *this - val);
}

Vec4 Vec4::operator *= (float val)
{
	return (*this = *this * val);
}

Vec4 Vec4::operator /= (float val)
{
	return (*this = *this / val);
}

Vec4 Vec4::operator += (Vec4 vec)
{
	return (*this = *this + vec);
}

Vec4 Vec4::operator -= (Vec4 vec)
{
	return (*this = *this - vec);
}

Vec4 Vec4::operator *= (Vec4 vec)
{
	return (*this = *this * vec);
}

Vec4 Vec4::operator /= (Vec4 vec)
{
	return (*this = *this / vec);
}
