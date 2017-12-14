#ifndef MAT2_H
# define MAT2_H

# include "Vec/Vec2.h"

class Mat2
{

	public:
		Vec2 data[2];
		Mat2(float val);
		Mat2() {};
		Vec2 &operator [] (int i);
		Mat2 operator * (Mat2 mat);
		Vec2 operator * (Vec2 vec);

};

#endif
