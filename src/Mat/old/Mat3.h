#ifndef MAT3_H
# define MAT3_H

# include "Vec/Vec3.h"

class Mat3
{

	public:
		Vec3 data[3];
		Mat3(float val);
		Mat3() {};
		static Mat3 rotateX(Mat3 mat, float angle);
		static Mat3 rotateY(Mat3 mat, float angle);
		static Mat3 rotateZ(Mat3 mat, float angle);
		Vec3 &operator [] (int i);
		Mat3 operator * (Mat3 mat);
		Vec3 operator * (Vec3 vec);

};

#endif
