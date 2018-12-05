#include "Vec2.h"
#include <algorithm>
#include <cmath>

float Vec2::dot(Vec2 uv)
{
	return (this->x * uv.x + this->y * uv.y);
}

float Vec2::angle(Vec2 vec)
{
	return (acos(dot(vec) / (length() * vec.length())));
}

float Vec2::length()
{
	return (sqrt(dot(*this)));
}

void Vec2::normalize()
{
	*this = *this / this->length();
}

void Vec2::min(float val)
{
	this->x = std::min(val, this->x);
	this->y = std::min(val, this->y);
}

void Vec2::max(float val)
{
	this->x = std::max(val, this->x);
	this->y = std::max(val, this->y);
}

void Vec2::clamp(float min, float max)
{
	this->max(min);
	this->min(max);
}

float &Vec2::operator [] (int idx)
{
	return (reinterpret_cast<float*>(this)[idx]);
}

Vec2 Vec2::operator + (float val)
{
	return (Vec2(this->x + val, this->y + val));
}

Vec2 Vec2::operator - (float val)
{
	return (Vec2(this->x - val, this->y - val));
}

Vec2 Vec2::operator * (float val)
{
	return (Vec2(this->x * val, this->y * val));
}

Vec2 Vec2::operator / (float val)
{
	return (Vec2(this->x / val, this->y / val));
}

Vec2 Vec2::operator + (Vec2 vec)
{
	return (Vec2(this->x + vec.x, this->y + vec.y));
}

Vec2 Vec2::operator - (Vec2 vec)
{
	return (Vec2(this->x - vec.x, this->y - vec.y));
}

Vec2 Vec2::operator * (Vec2 vec)
{
	return (Vec2(this->x * vec.x, this->y * vec.y));
}

Vec2 Vec2::operator / (Vec2 vec)
{
	return (Vec2(this->x / vec.x, this->y / vec.y));
}

Vec2 Vec2::operator += (float val)
{
	return (*this = *this + val);
}

Vec2 Vec2::operator -= (float val)
{
	return (*this = *this - val);
}

Vec2 Vec2::operator *= (float val)
{
	return (*this = *this * val);
}

Vec2 Vec2::operator /= (float val)
{
	return (*this = *this / val);
}

Vec2 Vec2::operator += (Vec2 vec)
{
	return (*this = *this + vec);
}

Vec2 Vec2::operator -= (Vec2 vec)
{
	return (*this = *this - vec);
}

Vec2 Vec2::operator *= (Vec2 vec)
{
	return (*this = *this * vec);
}

Vec2 Vec2::operator /= (Vec2 vec)
{
	return (*this = *this / vec);
}
