#include "Vec4.h"

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
