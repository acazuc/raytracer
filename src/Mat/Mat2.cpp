#include "Mat2.h"

Mat2::Mat2(float value)
{
	for (int i = 0; i < 2; ++i)
		this->data[i][i] = value;
}

Vec2 &Mat2::operator [] (int i)
{
	return (this->data[i]);
}

Mat2 Mat2::operator * (Mat2 mat)
{
	Mat2 result;
	for (int i = 0; i < 2; ++i)
		result[i] = (*this)[0] * mat[i][0] + (*this)[1] * mat[i][1];
	return (result);
}

Vec2 Mat2::operator * (Vec2 vec)
{
	Vec2 result;
	for (int i = 0; i < 2; ++i)
		result[i] = vec[0] * (*this)[0][i] + vec[1] * (*this)[1][i];
	return (result);
}
