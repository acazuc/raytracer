#ifndef VEC2_H
# define VEC2_H

class Vec2
{

	public:
		union {float x, r;};
		union {float y, g;};
		Vec2(float x, float y) : x(x), y(y) {};
		Vec2(float xy) : x(xy), y(xy) {};
		Vec2() : x(0), y(0) {};
		float dot(Vec2 vec);
		float angle(Vec2 vec);
		float length();
		void normalize();
		Vec2 operator + (float val);
		Vec2 operator - (float val);
		Vec2 operator * (float val);
		Vec2 operator / (float val);
		Vec2 operator + (Vec2 vec);
		Vec2 operator - (Vec2 vec);
		Vec2 operator * (Vec2 vec);
		Vec2 operator / (Vec2 vec);
		Vec2 operator += (float val);
		Vec2 operator -= (float val);
		Vec2 operator *= (float val);
		Vec2 operator /= (float val);
		Vec2 operator += (Vec2 vec);
		Vec2 operator -= (Vec2 vec);
		Vec2 operator *= (Vec2 vec);
		Vec2 operator /= (Vec2 vec);

};

#endif
