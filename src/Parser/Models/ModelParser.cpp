#include "ModelParser.h"

ModelParser::ModelParser(std::string filename)
: filename(filename)
{
}

void ModelParser::setPosition(Vec3 position)
{
	this->position = position;
}

void ModelParser::setRotation(Vec3 rotation)
{
	this->rotation = rotation;
}

void ModelParser::setScale(Vec3 scale)
{
	this->scale = scale;
}
