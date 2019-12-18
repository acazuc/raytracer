#ifndef MAT3_CPP
# define MAT3_CPP

#include "Mat3.h"
#include <cmath>

template <typename T>
TMat3<T>::TMat3(TVec3<T> vec1, TVec3<T> vec2, TVec3<T> vec3)
: data{vec1, vec2, vec3}
{
}

template <typename T>
TMat3<T>::TMat3(TVec3<T> vec)
: data{{vec.x, T(0), T(0)}, {T(0), vec.y, T(0)}, {T(0), T(0), T(0), vec.z}}
{
}

template <typename T>
TMat3<T>::TMat3(T value)
: data{{value, T(0), T(0)}, {T(0), value, T(0)}, {T(0), T(0), value}}
{
}

template <typename T>
TMat3<T> TMat3<T>::scale(TMat3<T> mat, TVec3<T> scale)
{
	return TMat3<T>(mat[0] * scale.x, mat[1] * scale.y, mat[2] * scale.z);
}

template <typename T>
TMat3<T> TMat3<T>::scale(TMat3<T> mat, T scale)
{
	return TMat3<T>(mat[0] * scale, mat[1] * scale, mat[2] * scale);
}

template <typename T>
TMat3<T> TMat3<T>::rotate(TMat3<T> mat, T angle, TVec3<T> axis)
{
	T c(cos(angle));
	T s(sin(angle));
	TVec3<T> t(axis * T(T(1) - c));
	TVec3<T> vx(t * axis.x);
	TVec3<T> u(axis * s);
	T yy(axis.y * t.y);
	T yz(axis.y * t.z);
	T zz(axis.z * t.z);
	axis.normalize();
	TMat3<T> rotate;
	rotate[0][0] = vx.x + c;
	rotate[0][1] = vx.y + u.z;
	rotate[0][2] = vx.z - u.y;
	rotate[1][0] = vx.y - u.x;
	rotate[1][1] = yy + c;
	rotate[1][2] = yz + u.z;
	rotate[2][0] = vx.z + u.y;
	rotate[2][1] = yz - u.x;
	rotate[2][2] = zz + c;
	return TMat3<T>(
			mat[0] * rotate[0][0] + mat[1] * rotate[0][1] + mat[2] * rotate[0][2],
			mat[0] * rotate[1][0] + mat[1] * rotate[1][1] + mat[2] * rotate[1][2],
			mat[0] * rotate[2][0] + mat[1] * rotate[2][1] + mat[2] * rotate[2][2]);
}

template <typename T>
TMat3<T> TMat3<T>::rotateX(TMat3<T> mat, T angle)
{
	T c(cos(angle));
	T s(sin(angle));
	TMat3<T> rotate(
			TVec3<T>(T(1), T( 0), T(0)),
			TVec3<T>(T(0),    c ,   s ),
			TVec3<T>(T(0),   -s ,   c ));
	return TMat3<T>(
			mat[0],
			mat[0] * rotate[1][0] + mat[1] * rotate[1][1] + mat[2] * rotate[1][2],
			mat[0] * rotate[2][0] + mat[1] * rotate[2][1] + mat[2] * rotate[2][2]);
}

template <typename T>
TMat3<T> TMat3<T>::rotateY(TMat3<T> mat, T angle)
{
	T c(cos(angle));
	T s(sin(angle));
	TMat3<T> rotate(
			TVec3<T>(  c , T(0),   -s ),
			TVec3<T>(T(0), T(1), T( 0)),
			TVec3<T>(  s , T(0),    c ));
	return TMat3<T>(
			mat[0] * rotate[0][0] + mat[1] * rotate[0][1] + mat[2] * rotate[0][2],
			mat[1],
			mat[0] * rotate[2][0] + mat[1] * rotate[2][1] + mat[2] * rotate[2][2]);
}

template <typename T>
TMat3<T> TMat3<T>::rotateZ(TMat3<T> mat, T angle)
{
	T c(cos(angle));
	T s(sin(angle));
	TMat3<T> rotate(
			TVec3<T>(   c ,   s , T(0)),
			TVec3<T>(  -s ,   c , T(0)),
			TVec3<T>(T( 0), T(0), T(1)));
	return TMat3<T>(
			mat[0] * rotate[0][0] + mat[1] * rotate[0][1] + mat[2] * rotate[0][2],
			mat[0] * rotate[1][0] + mat[1] * rotate[1][1] + mat[2] * rotate[1][2],
			mat[2]);
}

template <typename T>
TVec3<T> &TMat3<T>::operator [] (int i)
{
	return this->data[i];
}

template <typename T>
TMat3<T> operator * (TMat3<T> mat1, TMat3<T> mat2)
{
	TMat3<T> result;
	for (int i = 0; i < 3; ++i)
		result[i] = mat1[0] * mat2[i][0] + mat1[1] * mat2[i][1] + mat1[2] * mat2[i][2];
	return result;
}

template <typename T>
TVec3<T> operator * (TMat3<T> mat, TVec3<T> vec)
{
	TVec3<T> result;
	for (int i = 0; i < 3; ++i)
		result[i] = vec.x * mat[0][i] + vec.y * mat[1][i] + vec.z * mat[2][i];
	return result;
}

template <typename T>
TVec3<T> operator * (TVec3<T> vec, TMat3<T> mat)
{
	TVec3<T> result;
	for (int i = 0; i < 3; ++i)
		result[i] = vec.x * mat[i].x + vec.y * mat[i].y + vec.z * mat[i].z;
	return result;
}

template <typename T>
TMat3<T> transpose(TMat3<T> mat)
{
	return TMat3<T>(
			TVec3<T>(mat[0][0], mat[1][0], mat[2][0]),
			TVec3<T>(mat[0][1], mat[1][1], mat[2][1]),
			TVec3<T>(mat[0][2], mat[1][2], mat[2][2]));
}

template <typename T>
TMat3<T> inverse(TMat3<T> mat)
{
	T deter(
			mat[0][0] * (mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2])
			- mat[1][0] * (mat[0][1] * mat[2][2] - mat[2][1] * mat[0][2])
			+ mat[2][0] * (mat[0][1] * mat[1][2] - mat[1][1] * mat[0][2]));
	return TMat3<T>(
			TVec3<T>(
				(mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]) / deter,
				-(mat[0][1] * mat[2][2] - mat[0][2] * mat[2][1]) / deter,
				(mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1]) / deter),
			TVec3<T>(
				-(mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0]) / deter,
				(mat[0][0] * mat[2][2] - mat[0][2] * mat[2][0]) / deter,
				-(mat[0][0] * mat[1][2] - mat[0][2] * mat[1][0]) / deter),
			TVec3<T>(
				(mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]) / deter,
				-(mat[0][0] * mat[2][1] - mat[0][1] * mat[2][0]) / deter,
				(mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0]) / deter));
}

#endif
