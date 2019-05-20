#include "Object.h"

Object::Object()
: material(nullptr)
, position(0)
, rotation(0)
, scale(1)
, invMat(1)
, mat(1)
{
}

void Object::calcMatrix()
{
	this->mat = Mat3(1);
	this->mat = Mat3::scale(this->mat, this->scale);
	this->mat = Mat3::rotateZ(this->mat, this->rotation.z);
	this->mat = Mat3::rotateY(this->mat, this->rotation.y);
	this->mat = Mat3::rotateX(this->mat, this->rotation.x);
	this->invMat = Mat3(1);
	this->invMat = Mat3::rotateX(this->invMat, -this->rotation.x);
	this->invMat = Mat3::rotateY(this->invMat, -this->rotation.y);
	this->invMat = Mat3::rotateZ(this->invMat, -this->rotation.z);
	this->invMat = Mat3::scale(this->invMat, Vec3(1.f) / this->scale);
}

void Object::setMaterial(Material *material)
{
	this->material = material;
}

void Object::setPosition(Vec3 position)
{
	this->position = position;
	calcMatrix();
}

void Object::setRotation(Vec3 rotation)
{
	this->rotation = rotation;
	calcMatrix();
}

void Object::setScale(Vec3 scale)
{
	this->scale = scale;
	calcMatrix();
}
