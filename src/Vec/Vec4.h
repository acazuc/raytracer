#ifndef VEC4_H
# define VEC4_H

# include "Vec3.h"
# include "Vec2.h"

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
		float dot(Vec4 vec);
		float angle(Vec4 vec);
		float length();
		void normalize();
		Vec3 xyz();
		Vec4 operator + (float val);
		Vec4 operator - (float val);
		Vec4 operator * (float val);
		Vec4 operator / (float val);
		Vec4 operator + (Vec4 vec);
		Vec4 operator - (Vec4 vec);
		Vec4 operator * (Vec4 vec);
		Vec4 operator / (Vec4 vec);
		Vec4 operator += (float val);
		Vec4 operator -= (float val);
		Vec4 operator *= (float val);
		Vec4 operator /= (float val);
		Vec4 operator += (Vec4 vec);
		Vec4 operator -= (Vec4 vec);
		Vec4 operator *= (Vec4 vec);
		Vec4 operator /= (Vec4 vec);

};

#endif
