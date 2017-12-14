#include "Mat4.h"

Mat4::Mat4(float value)
{
	for (int i = 0; i < 4; ++i)
		this->data[i][i] = value;
}

Vec4 &Mat4::operator [] (int i)
{
	return (this->data[i]);
}

Mat4 Mat4::operator * (Mat4 mat)
{
	Mat4 result;
	for (int i = 0; i < 4; ++i)
		result[i] = (*this)[0] * mat[i][0] + (*this)[1] * mat[i][1] + (*this)[2] * mat[i][2] + (*this)[3] * mat[i][3];
	return (result);
}

Vec4 Mat4::operator * (Vec4 vec)
{
	Vec4 result;
	for (int i = 0; i < 4; ++i)
		result[i] = vec[0] * (*this)[0][i] + vec[1] * (*this)[1][i] + vec[2] * (*this)[2][i] + vec[3] * (*this)[3][i];
	return (result);
}
