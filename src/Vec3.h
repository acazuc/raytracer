#ifndef VEC3_H
# define VEC3_H

# include "Vec4.h"

class Vec3
{

	public:
		union {float x, r;};
		union {float y, g;};
		union {float z, b;};
		Vec3(Vec4 vec) : x(vec.x), y(vec.y), z(vec.z) {};
		Vec3(float x, float y, float z) : x(x), y(y), z(z) {};
		Vec3(float xyz) : x(xyz), y(xyz), z(xyz) {};
		Vec3() {};
		Vec3 reflect(Vec3 vec);
		float dot(Vec3 vec);
		float angle(Vec3 vec);
		float length();
		void normalize();
		void rotate(Vec3 vec);
		void rotateX(float angle);
		void rotateY(float angle);
		void rotateZ(float angle);
		Vec3 operator - ();
		Vec3 operator + (float val);
		Vec3 operator - (float val);
		Vec3 operator * (float val);
		Vec3 operator / (float val);
		Vec3 operator + (Vec3 vec);
		Vec3 operator - (Vec3 vec);
		Vec3 operator * (Vec3 vec);
		Vec3 operator / (Vec3 vec);
		Vec3 operator += (float val);
		Vec3 operator -= (float val);
		Vec3 operator *= (float val);
		Vec3 operator /= (float val);
		Vec3 operator += (Vec3 vec);
		Vec3 operator -= (Vec3 vec);
		Vec3 operator *= (Vec3 vec);
		Vec3 operator /= (Vec3 vec);

};

#endif
