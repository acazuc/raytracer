#include "Object.h"

Object::Object()
: material(nullptr)
, invMat(1)
, mat(1)
{
}

void Object::setMaterial(Material *material)
{
	this->material = material;
}

void Object::setPosition(Vec3 position)
{
	this->position = position;
}

void Object::setRotation(Vec3 rotation)
{
	//this->rot = vec;
	this->mat = Mat3(1);
	this->mat = Mat3::rotateZ(this->mat, rotation.z);
	this->mat = Mat3::rotateY(this->mat, rotation.y);
	this->mat = Mat3::rotateX(this->mat, rotation.x);
	this->invMat = Mat3(1);
	this->invMat = Mat3::rotateX(this->invMat, -rotation.x);
	this->invMat = Mat3::rotateY(this->invMat, -rotation.y);
	this->invMat = Mat3::rotateZ(this->invMat, -rotation.z);
}
