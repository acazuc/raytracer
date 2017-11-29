#include "Vec2.h"

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
