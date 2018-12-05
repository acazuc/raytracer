#ifndef VEC4_H
# define VEC4_H

# include "Vec3.h"
# include "Vec2.h"
# include <cmath>

class Vec4
{

	public:
		union {float x, r;};
		union {float y, g;};
		union {float z, b;};
		union {float w, a;};
		Vec4(Vec2 vec1, Vec2 vec2) : x(vec1.x), y(vec1.y), z(vec2.x), w(vec2.y) {};
		Vec4(Vec2 vec1, float z, float w) : x(vec1.x), y(vec1.y), z(z), w(w) {};
		Vec4(Vec3 vec, float w) : x(vec.x), y(vec.y), z(vec.z), w(w) {};
		Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};
		Vec4(float xyz, float w) : x(xyz), y(xyz), z(xyz), w(w) {};
		Vec4(float xyzw) : x(xyzw), y(xyzw), z(xyzw), w(xyzw) {};
		Vec4() : x(0), y(0), z(0), w(0) {};
		float dot(Vec4 vec) {return this->x * vec.x + this->y * vec.y + this->z * vec.z + this->w * vec.w;};
		float angle(Vec4 vec) {return acos(dot(vec) / (length() * vec.length()));};
		float length() {return sqrt(dot(*this));};
		void normalize() {*this = *this / this->length();};
		Vec3 xyz() {return Vec3(this->x, this->y, this->z);};
		Vec3 rgb() {return Vec3(this->r, this->g, this->b);};
		Vec2 xy() {return Vec2(this->x, this->y);};
		Vec2 zw() {return Vec2(this->z, this->w);};
		float &operator [] (int idx) {return reinterpret_cast<float*>(this)[idx];};
		Vec4 operator + (float val) {return Vec4(this->x + val, this->y + val, this->z + val, this->w + val);};
		Vec4 operator - (float val) {return Vec4(this->x - val, this->y - val, this->z - val, this->w - val);};
		Vec4 operator * (float val) {return Vec4(this->x * val, this->y * val, this->z * val, this->w * val);};
		Vec4 operator / (float val) {return Vec4(this->x / val, this->y / val, this->z / val, this->w / val);};
		Vec4 operator + (Vec4 vec) {return Vec4(this->x + vec.x, this->y + vec.y, this->z + vec.z, this->w + vec.w);};
		Vec4 operator - (Vec4 vec) {return Vec4(this->x - vec.x, this->y - vec.y, this->z - vec.z, this->w - vec.w);};
		Vec4 operator * (Vec4 vec) {return Vec4(this->x * vec.x, this->y * vec.y, this->z * vec.z, this->w * vec.w);};
		Vec4 operator / (Vec4 vec) {return Vec4(this->x / vec.x, this->y / vec.y, this->z / vec.z, this->w / vec.w);};
		Vec4 operator += (float val) {return *this = *this + val;};
		Vec4 operator -= (float val) {return *this = *this - val;};
		Vec4 operator *= (float val) {return *this = *this * val;};
		Vec4 operator /= (float val) {return *this = *this / val;};
		Vec4 operator += (Vec4 vec) {return *this = *this + vec;};
		Vec4 operator -= (Vec4 vec) {return *this = *this - vec;};
		Vec4 operator *= (Vec4 vec) {return *this = *this * vec;};
		Vec4 operator /= (Vec4 vec) {return *this = *this / vec;};

};

#endif
