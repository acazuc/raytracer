#ifndef VEC3_H
# define VEC3_H

# include "./Vec2.h"

template <typename T> class TVec3
{

	public:
		union {T x, r;};
		union {T y, g;};
		union {T z, b;};
		inline TVec3<T>(TVec2<T> vec, T z) : x(vec.x), y(vec.y), z(z) {};
		inline TVec3<T>(T x, T y, T z) : x(x), y(y), z(z) {};
		inline TVec3<T>(T xyz) : x(xyz), y(xyz), z(xyz) {};
		inline TVec3<T>() {};
		T &operator [] (int idx);
		TVec3<T> operator - ();
		TVec3<T> operator += (T val);
		TVec3<T> operator -= (T val);
		TVec3<T> operator *= (T val);
		TVec3<T> operator /= (T val);
		TVec3<T> operator += (TVec3<T> vec);
		TVec3<T> operator -= (TVec3<T> vec);
		TVec3<T> operator *= (TVec3<T> vec);
		TVec3<T> operator /= (TVec3<T> vec);
		bool operator == (TVec3<T> vec);
		bool operator != (TVec3<T> vec);

};

template <typename T>
TVec3<T> operator + (TVec3<T> vec1, TVec3<T> vec2);
template <typename T>
TVec3<T> operator + (TVec3<T> vec, T val);
template <typename T>
TVec3<T> operator + (T val, TVec3<T> vec);
template <typename T>
TVec3<T> operator - (TVec3<T> vec1, TVec3<T> vec2);
template <typename T>
TVec3<T> operator - (TVec3<T> vec, T val);
template <typename T>
TVec3<T> operator - (T val, TVec3<T> vec);
template <typename T>
TVec3<T> operator * (TVec3<T> vec1, TVec3<T> vec2);
template <typename T>
TVec3<T> operator * (TVec3<T> vec, T val);
template <typename T>
TVec3<T> operator * (T val, TVec3<T> vec);
template <typename T>
TVec3<T> operator / (TVec3<T> vec1, TVec3<T> vec2);
template <typename T>
TVec3<T> operator / (TVec3<T> vec, T val);
template <typename T>
TVec3<T> operator / (T val, TVec3<T> vec);

template <typename T>
TVec3<T> min(TVec3<T> vec1, TVec3<T> vec2);
template <typename T>
TVec3<T> min(TVec3<T> vec, T val);
template <typename T>
TVec3<T> min(T val, TVec3<T> vec);

template <typename T>
TVec3<T> max(TVec3<T> vec1, TVec3<T> vec2);
template <typename T>
TVec3<T> max(TVec3<T> vec, T val);
template <typename T>
TVec3<T> max(T val, TVec3<T> vec);

template <typename T>
TVec3<T> clamp(TVec3<T> vec, T min, T max);
template <typename T>
TVec3<T> clamp(TVec3<T> vec, TVec3<T> min, TVec3<T> max);

template <typename T>
TVec3<T> mix(TVec3<T> vec1, TVec3<T> vec2, T per);
template <typename T>
TVec3<T> mod(TVec3<T> vec, T val);
template <typename T>
TVec3<T> floor(TVec3<T> vec);
template <typename T>
TVec3<T> round(TVec3<T> vec);
template <typename T>
TVec3<T> ceil(TVec3<T> vec);
template <typename T>
TVec3<T> fract(TVec3<T> vec);

template<typename T>
TVec3<T> normalize(TVec3<T> vec);
template<typename T>
TVec3<T> reflect(TVec3<T> vec1, TVec3<T> vec2);
template<typename T>
T dot(TVec3<T> vec1, TVec3<T> vec2);
template<typename T>
T angle(TVec3<T> vec1, TVec3<T> vec2);
template<typename T>
T length(TVec3<T> vec);
template<typename T>
TVec3<T> cross(TVec3<T> vec1, TVec3<T> vec2);

typedef TVec3<float> Vec3;

# include "Vec3.cpp"

#endif
