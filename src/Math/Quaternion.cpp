#ifndef QUATERNION_CPP
# define QUATERNION_CPP

template <typename T>
TQuaternion<T>::TQuaternion(T x, T y, T z, T w)
: x(x)
, y(y)
, z(z)
, w(w)
{
}

template <typename T>
TQuaternion<T>::TQuaternion(TVec3<T> zyxEuler)
{
	zyxEuler *= T(.5f);
	float cz = cos(zyxEuler.z);
	float sz = sin(zyxEuler.z);
	float cy = cos(zyxEuler.y);
	float sy = sin(zyxEuler.y);
	float cx = cos(zyxEuler.x);
	float sx = sin(zyxEuler.x);
	float czcy = cz * cy;
	float szcy = sz * cy;
	float szsy = sz * sy;
	float czsy = cz * sy;
	this->w = czcy * cx + szsy * sx;
	this->x = czcy * sx - szsy * cx;
	this->y = szcy * sx + czsy * cx;
	this->z = szcy * cx - czsy * sx;
}

template<typename T>
TQuaternion<T> inverse(TQuaternion<T> quaternion)
{
	return TQuaternion<T>(quaternion.x, -quaternion.y, -quaternion.z, -quaternion.w);
}

template<typename T>
TVec3<T> operator * (TQuaternion<T> quaternion, TVec3<T> vec)
{
	TVec3 u(quaternion.x, quaternion.y, quaternion.z);
	float s = quaternion.w;
	return T(2) * dot(u, vec) * u + (s * s - dot(u, u)) * vec + T(2) * s * cross(u, vec);
}

#endif
