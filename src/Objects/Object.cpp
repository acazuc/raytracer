#include "Object.h"

Object::Object()
: Kd_map(NULL)
, N_map(NULL)
, unrotMat(1)
, rotMat(1)
, Ka(0)
, Kd(1)
, Ks(0)
, Ns(100)
, Ni(1)
, Ir(0)
{
	//Empty
}

void Object::setRot(Vec3 vec)
{
	//this->rot = vec;
	Mat3 rot(1);
	rot = Mat3::rotateZ(rot, vec.z);
	rot = Mat3::rotateY(rot, vec.y);
	rot = Mat3::rotateX(rot, vec.x);
	this->rotMat = rot;
	Mat3 unrot(1);
	unrot = Mat3::rotateX(unrot, -vec.x);
	unrot = Mat3::rotateY(unrot, -vec.y);
	unrot = Mat3::rotateZ(unrot, -vec.z);
	this->unrotMat = unrot;

}
