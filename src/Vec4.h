#ifndef VEC4_H
# define VEC4_H

class Vec4
{

	public:
		union {float x, r;};
		union {float y, g;};
		union {float z, b;};
		union {float w, a;};
		Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};
		Vec4(float xyz, float w) : x(xyz), y(xyz), z(xyz), w(w) {};
		Vec4(float xyzw) : x(xyzw), y(xyzw), z(xyzw), w(xyzw) {};
		Vec4() : x(0), y(0), z(0), w(0) {};
		Vec4 operator + (float val);
		Vec4 operator - (float val);
		Vec4 operator * (float val);
		Vec4 operator / (float val);
		Vec4 operator + (Vec4 vec);
		Vec4 operator - (Vec4 vec);
		Vec4 operator * (Vec4 vec);
		Vec4 operator / (Vec4 vec);

};

#endif
