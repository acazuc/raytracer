#include "Mat3.h"
#include <cmath>

Mat3::Mat3(float value)
{
	for (int i = 0; i < 3; ++i)
		this->data[i][i] = value;
}

Vec3 &Mat3::operator [] (int i)
{
	return (this->data[i]);
}

Mat3 Mat3::rotateX(Mat3 mat, float angle)
{
	float ca = cos(angle);
	float sa = sin(angle);
	Mat3 rotate(0);
	rotate[0] = Vec3(1, 0 ,  0 );
	rotate[1] = Vec3(0, ca, -sa);
	rotate[2] = Vec3(0, sa,  ca);
	return (mat * rotate);
}

Mat3 Mat3::rotateY(Mat3 mat, float angle)
{
	float ca = cos(angle);
	float sa = sin(angle);
	Mat3 rotate(0);
	rotate[0] = Vec3( ca, 0, sa);
	rotate[1] = Vec3( 0 , 1, 0 );
	rotate[2] = Vec3(-sa, 0, ca);
	return (mat * rotate);
}

Mat3 Mat3::rotateZ(Mat3 mat, float angle)
{
	float ca = cos(angle);
	float sa = sin(angle);
	Mat3 rotate(0);
	rotate[0] = Vec3(ca, -sa, 0);
	rotate[1] = Vec3(sa,  ca, 0);
	rotate[2] = Vec3(0 ,  0 , 1);
	return (mat * rotate);
}

Mat3 Mat3::operator * (Mat3 mat)
{
	Mat3 result;
	for (int i = 0; i < 3; ++i)
		result[i] = (*this)[0] * mat[i][0] + (*this)[1] * mat[i][1] + (*this)[2] * mat[i][2];
	return (result);
}

Vec3 Mat3::operator * (Vec3 vec)
{
	Vec3 result;
	for (int i = 0; i < 3; ++i)
		result[i] = vec[0] * (*this)[0][i] + vec[1] * (*this)[1][i] + vec[2] * (*this)[2][i];
	return (result);
}
