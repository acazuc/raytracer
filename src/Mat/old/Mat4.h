#ifndef MAT4_H
# define MAT4_H

# include "Vec/Vec4.h"

class Mat4
{

	public:
		Vec4 data[4];
		Mat4(float val);
		Mat4() {};
		Vec4 &operator [] (int i);
		Mat4 operator * (Mat4 mat);
		Vec4 operator * (Vec4 vec);

};

#endif
