#ifndef VEC3_H
# define VEC3_H

class Vec3
{

	public:
		union {float x, r;};
		union {float y, g;};
		union {float z, b;};
		Vec3(float x, float y, float z) : x(x), y(y), z(z) {};
		Vec3(float xyz) : x(xyz), y(xyz), z(xyz) {};
		Vec3() {};
		float dot(Vec3 vec);
		float length();
		Vec3 operator + (float val);
		Vec3 operator - (float val);
		Vec3 operator * (float val);
		Vec3 operator / (float val);
		Vec3 operator + (Vec3 vec);
		Vec3 operator - (Vec3 vec);
		Vec3 operator * (Vec3 vec);
		Vec3 operator / (Vec3 vec);

};

#endif
