#ifndef VEC3_H
# define VEC3_H

# include "Vec2.h"
# include <cmath>

class Vec3
{

	public:
		union {float x, r;};
		union {float y, g;};
		union {float z, b;};
		Vec3(Vec2 vec, float z) : x(vec.x), y(vec.y), z(z) {};
		Vec3(float x, float y, float z) : x(x), y(y), z(z) {};
		Vec3(float xyz) : x(xyz), y(xyz), z(xyz) {};
		Vec3() : x(0), y(0), z(0) {};
		Vec3 reflect(Vec3 vec) {return *this - vec * 2 * this->dot(vec);};
		Vec3 cross(Vec3 vec) {return Vec3(this->y * vec.z - this->z * vec.y, this->z * vec.x - this->x * vec.z, this->x * vec.y - this->y * vec.x);};
		float dot(Vec3 vec) {return this->x * vec.x + this->y * vec.y + this->z * vec.z;};
		float angle(Vec3 vec) {return acos(dot(vec) / (length() * vec.length()));};
		float length() {return sqrt(dot(*this));};
		void normalize() {*this /= this->length();};
		void rotate(Vec3 vec) {rotateX(vec.x);rotateY(vec.y);rotateZ(vec.z);};
		void unrotate(Vec3 vec) {rotateZ(-vec.z);rotateY(-vec.y);rotateX(-vec.x);};
		void rotateX(float angle);
		void rotateY(float angle);
		void rotateZ(float angle);
		float &operator [] (int idx) {return reinterpret_cast<float*>(this)[idx];};
		Vec3 operator - () {return Vec3(-this->x, -this->y, -this->z);};
		Vec3 operator + (float val) {return Vec3(this->x + val, this->y + val, this->z + val);};
		Vec3 operator - (float val) {return Vec3(this->x - val, this->y - val, this->z - val);};
		Vec3 operator * (float val) {return Vec3(this->x * val, this->y * val, this->z * val);};
		Vec3 operator / (float val) {return Vec3(this->x / val, this->y / val, this->z / val);};
		Vec3 operator + (Vec3 vec) {return Vec3(this->x + vec.x, this->y + vec.y, this->z + vec.z);};
		Vec3 operator - (Vec3 vec) {return Vec3(this->x - vec.x, this->y - vec.y, this->z - vec.z);};
		Vec3 operator * (Vec3 vec) {return Vec3(this->x * vec.x, this->y * vec.y, this->z * vec.z);};
		Vec3 operator / (Vec3 vec) {return Vec3(this->x / vec.x, this->y / vec.y, this->z / vec.z);};
		Vec3 operator += (float val) {return *this = *this + val;};
		Vec3 operator -= (float val) {return *this = *this - val;};
		Vec3 operator *= (float val) {return *this = *this * val;};
		Vec3 operator /= (float val) {return *this = *this / val;};
		Vec3 operator += (Vec3 vec) {return *this = *this + vec;};
		Vec3 operator -= (Vec3 vec) {return *this = *this - vec;};
		Vec3 operator *= (Vec3 vec) {return *this = *this * vec;};
		Vec3 operator /= (Vec3 vec) {return *this = *this / vec;};

};

#endif
