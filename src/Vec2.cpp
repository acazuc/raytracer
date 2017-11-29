#include "Vec2.h"
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
	this->x += val;
	this->y += val;
	return (*this);
}

Vec2 Vec2::operator -= (float val)
{
	this->x -= val;
	this->y -= val;
	return (*this);
}

Vec2 Vec2::operator *= (float val)
{
	this->x *= val;
	this->y *= val;
	return (*this);
}

Vec2 Vec2::operator /= (float val)
{
	this->x /= val;
	this->y /= val;
	return (*this);
}

Vec2 Vec2::operator += (Vec2 vec)
{
	this->x += vec.x;
	this->y += vec.y;
	return (*this);
}

Vec2 Vec2::operator -= (Vec2 vec)
{
	this->x -= vec.x;
	this->y -= vec.y;
	return (*this);
}

Vec2 Vec2::operator *= (Vec2 vec)
{
	this->x *= vec.x;
	this->y *= vec.y;
	return (*this);
}

Vec2 Vec2::operator /= (Vec2 vec)
{
	this->x /= vec.x;
	this->y /= vec.y;
	return (*this);
}
