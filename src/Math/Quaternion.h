#ifndef QUATERNION_H
# define QUATERNION_H

# include "Vec3.h"

template <typename T> class TQuaternion
{

	public:
		union {T x;};
		union {T y;};
		union {T z;};
		union {T w;};
		TQuaternion<T>(T x, T y, T z, T w);
		TQuaternion<T>(TVec3<T> zyxEuler);
		TQuaternion<T>() {};

};

template <typename T>
TQuaternion<T> inverse(TQuaternion<T> quaternion);

template <typename T>
TVec3<T> operator * (TQuaternion<T> quaternion, TVec3<T> vec);

typedef TQuaternion<float> Quaternion;

# include "Quaternion.cpp"

#endif
