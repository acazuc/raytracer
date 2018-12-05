#ifndef MAT2_CPP
# define MAT2_CPP

#include "Mat2.h"

template <typename T>
TMat2<T>::TMat2(TVec2<T> vec1, TVec2<T> vec2)
	: data{vec1, vec2}
{
	//Empty
}

template <typename T>
TMat2<T>::TMat2(TVec2<T> vec)
	: data{{vec.x, 0}, {0, vec.y}}
{
	//Empty
}

template <typename T>
TMat2<T>::TMat2(T value)
	: data{{value, 0}, {0, value}}
{
	//Empty
}

template <typename T>
TVec2<T> &TMat2<T>::operator [] (int i)
{
	return this->data[i];
}

template <typename T>
TMat2<T> operator * (TMat2<T> mat1, TMat2<T> mat2)
{
	TMat2<T> result;
	for (int i = 0; i < 2; ++i)
		result[i] = mat1[0] * mat2[i][0] + mat1[1] * mat2[i][1];
	return result;
}

template <typename T>
TVec2<T> operator * (TMat2<T> mat, TVec2<T> vec)
{
	TVec2<T> result;
	for (int i = 0; i < 2; ++i)
		result[i] = vec.x * mat[0][i] + vec.y * mat[1][i];
	return result;
}

template <typename T>
TVec2<T> operator * (TVec2<T> vec, TMat2<T> mat)
{
	TVec2<T> result;
	for (int i = 0; i < 2; ++i)
		result[i] = vec.x * mat[i].x + vec.y * mat[i].y;
	return result;
}

template <typename T>
TMat2<T> transpose(TMat2<T> mat)
{
	return TMat2<T>(
			TVec2<T>(mat[0][0], mat[1][0]),
			TVec2<T>(mat[0][1], mat[1][1]));
}

template <typename T>
TMat2<T> inverse(TMat2<T> mat)
{
	T deter(mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1]);
	return TMat2<T>(
			TVec2<T>( mat[1][1] / deter, -mat[0][1] / deter),
			TVec2<T>(-mat[1][0] / deter,  mat[0][0] / deter));
}

#endif
