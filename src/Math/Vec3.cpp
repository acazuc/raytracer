#ifndef VEC3_CPP
# define VEC3_CPP

#include "Vec3.h"
#include <algorithm>
#include <cmath>

template <typename T>
inline T &TVec3<T>::operator [] (int idx)
{
	return reinterpret_cast<T*>(this)[idx];
}

template <typename T>
inline TVec3<T> TVec3<T>::operator - ()
{
	return TVec3<T>(-this->x, -this->y, -this->z);
}

template <typename T>
inline TVec3<T> TVec3<T>::operator += (T val)
{
	return *this = *this + val;
}

template <typename T>
inline TVec3<T> TVec3<T>::operator -= (T val)
{
	return *this = *this - val;
}

template <typename T>
inline TVec3<T> TVec3<T>::operator *= (T val)
{
	return *this = *this * val;
}

template <typename T>
inline TVec3<T> TVec3<T>::operator /= (T val)
{
	return *this = *this / val;
}

template <typename T>
inline TVec3<T> TVec3<T>::operator += (TVec3<T> vec)
{
	return *this = *this + vec;
}

template <typename T>
inline TVec3<T> TVec3<T>::operator -= (TVec3<T> vec)
{
	return *this = *this - vec;
}

template <typename T>
inline TVec3<T> TVec3<T>::operator *= (TVec3<T> vec)
{
	return *this = *this * vec;
}

template <typename T>
inline TVec3<T> TVec3<T>::operator /= (TVec3<T> vec)
{
	return *this = *this / vec;
}

template <typename T>
inline bool TVec3<T>::operator == (TVec3<T> vec)
{
	return this->x == vec.x && this->y == vec.y && this->z == vec.z;
}

template <typename T>
inline bool TVec3<T>::operator != (TVec3<T> vec)
{
	return !(*this == vec);
}

template <typename T>
inline TVec3<T> operator + (TVec3<T> vec1, TVec3<T> vec2)
{
	return TVec3<T>(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z);
}

template <typename T>
inline TVec3<T> operator + (TVec3<T> vec, T val)
{
	return TVec3<T>(vec.x + val, vec.y + val, vec.z + val);
}

template <typename T>
inline TVec3<T> operator + (T val, TVec3<T> vec)
{
	return TVec3<T>(val + vec.x, val + vec.y, val + vec.z);
}

template <typename T>
inline TVec3<T> operator - (TVec3<T> vec1, TVec3<T> vec2)
{
	return TVec3<T>(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z);
}

template <typename T>
inline TVec3<T> operator - (TVec3<T> vec, T val)
{
	return TVec3<T>(vec.x - val, vec.y - val, vec.z - val);
}

template <typename T>
inline TVec3<T> operator - (T val, TVec3<T> vec)
{
	return TVec3<T>(val - vec.x, val - vec.y, val - vec.z);
}

template <typename T>
inline TVec3<T> operator * (TVec3<T> vec1, TVec3<T> vec2)
{
	return TVec3<T>(vec1.x * vec2.x, vec1.y * vec2.y, vec1.z * vec2.z);
}

template <typename T>
inline TVec3<T> operator * (TVec3<T> vec, T val)
{
	return TVec3<T>(vec.x * val, vec.y * val, vec.z * val);
}

template <typename T>
inline TVec3<T> operator * (T val, TVec3<T> vec)
{
	return TVec3<T>(val * vec.x, val * vec.y, val * vec.z);
}

template <typename T>
inline TVec3<T> operator / (TVec3<T> vec1, TVec3<T> vec2)
{
	return TVec3<T>(vec1.x / vec2.x, vec1.y / vec2.y, vec1.z / vec2.z);
}

template <typename T>
inline TVec3<T> operator / (TVec3<T> vec, T val)
{
	return TVec3<T>(vec.x / val, vec.y / val, vec.z / val);
}

template <typename T>
inline TVec3<T> operator / (T val, TVec3<T> vec)
{
	return TVec3<T>(val / vec.x, val / vec.y, val / vec.z);
}

template <typename T>
inline TVec3<T> min(TVec3<T> vec1, TVec3<T> vec2)
{
	return TVec3<T>(std::min(vec1.x, vec2.x), std::min(vec1.y, vec2.y), std::min(vec1.z, vec2.z));
}

template <typename T>
inline TVec3<T> min(TVec3<T> vec, T val)
{
	return TVec3<T>(std::min(val, vec.x), std::min(val, vec.y), std::min(val, vec.z));
}

template <typename T>
inline TVec3<T> min(T val, TVec3<T> vec)
{
	return TVec3<T>(std::min(val, vec.x), std::min(val, vec.y), std::min(val, vec.z));
}

template <typename T>
inline TVec3<T> max(TVec3<T> vec1, TVec3<T> vec2)
{
	return TVec3<T>(std::max(vec1.x, vec2.x), std::max(vec1.y, vec2.y), std::max(vec1.z, vec2.z));
}

template <typename T>
inline TVec3<T> max(TVec3<T> vec, T val)
{
	return TVec3<T>(std::max(val, vec.x), std::max(val, vec.y), std::max(val, vec.z));
}

template <typename T>
inline TVec3<T> max(T val, TVec3<T> vec)
{
	return TVec3<T>(std::max(val, vec.x), std::max(val, vec.y), std::max(val, vec.z));
}

template <typename T>
inline TVec3<T> clamp(TVec3<T> vec, T vmin, T vmax)
{
	return max(vmin, min(vec, vmax));
}

template <typename T>
inline TVec3<T> clamp(TVec3<T> vec, TVec3<T> vmin, TVec3<T> vmax)
{
	return max(vmin, min(vec, vmax));
}

template <typename T>
inline TVec3<T> mix(TVec3<T> vec1, TVec3<T> vec2, T a)
{
	return vec1 * (T(1) - a) + vec2 * a;
}

template <typename T>
inline TVec3<T> mod(TVec3<T> vec, T val)
{
	return TVec3<T>(std::fmod(vec.x, val), std::fmod(vec.y, val), std::fmod(vec.z, val));
}

template <typename T>
inline TVec3<T> floor(TVec3<T> vec)
{
	return TVec3<T>(std::floor(vec.x), std::floor(vec.y), std::floor(vec.z));
}

template <typename T>
inline TVec3<T> round(TVec3<T> vec)
{
	return TVec3<T>(std::round(vec.x), std::round(vec.y), std::round(vec.z));
}

template <typename T>
inline TVec3<T> ceil(TVec3<T> vec)
{
	return TVec3<T>(std::ceil(vec.x), std::ceil(vec.y), std::ceil(vec.z));
}

template <typename T>
inline TVec3<T> fract(TVec3<T> vec)
{
	return vec - floor(vec);
}

template<typename T>
inline TVec3<T> normalize(TVec3<T> vec)
{
	return vec / length(vec);
}

template<typename T>
inline TVec3<T> reflect(TVec3<T> vec1, TVec3<T> vec2)
{
	return vec1 - vec2 * T(2) * dot(vec1, vec2);
}

template<typename T>
inline T dot(TVec3<T> vec1, TVec3<T> vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

template<typename T>
inline T angle(TVec3<T> vec1, TVec3<T> vec2)
{
	return acos(dot(vec1, vec2) / length(vec1) / length(vec2));
}

template<typename T>
inline T length(TVec3<T> vec)
{
	return sqrt(dot(vec, vec));
}

template <typename T>
inline TVec3<T> cross(TVec3<T> vec1, TVec3<T> vec2)
{
	return TVec3<T>(vec1.y * vec2.z - vec1.z * vec2.y, vec1.z * vec2.x - vec1.x * vec2.z, vec1.x * vec2.y - vec1.y * vec2.x);
}

#endif
