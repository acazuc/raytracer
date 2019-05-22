#include "ObjectsParser.h"
#include "Objects/Triangle.h"
#include "Objects/Cylinder.h"
#include "Objects/Sphere.h"
#include "Objects/Plane.h"
#include "Objects/Cone.h"
#include "./FileParser.h"

ObjectParser::ObjectParser(FileParser &file, GroupParser &parent)
: parent(parent)
, file(file)
, position(0)
, rotation(0)
, scale(1)
{
	registerAttributeCallback("material", ParserAttributeCallback(&ObjectParser::parseMaterial));
	registerNodeCallback("Position", ParserNodeCallback(&ObjectParser::parsePosition));
	registerNodeCallback("Rotation", ParserNodeCallback(&ObjectParser::parseRotation));
	registerNodeCallback("Scale", ParserNodeCallback(&ObjectParser::parseScale));
}

void ObjectParser::parseMaterial(xmlAttr *attr)
{
	parseAttrString(attr, &this->material);
}

void ObjectParser::parsePosition(xmlNode *node)
{
	parseVec3(node, &this->position);
}

void ObjectParser::parseRotation(xmlNode *node)
{
	parseVec3(node, &this->rotation);
}

void ObjectParser::parseScale(xmlNode *node)
{
	parseVec3(node, &this->scale);
}

ObjectParser::~ObjectParser()
{
}

SphereParser::SphereParser(FileParser &file, GroupParser &parent)
: ObjectParser(file, parent)
, size(1)
{
	registerAttributeCallback("size", ParserAttributeCallback(&SphereParser::parseSize));
}

void SphereParser::parseSize(xmlAttr *attr)
{
	parseAttrFloat(attr, &this->size);
}

Object *SphereParser::toObject()
{
	Sphere *sphere = new Sphere(this->size);
	sphere->setMaterial(this->file.getMaterial(this->material));
	sphere->setPosition(this->position);
	sphere->setRotation(this->rotation);
	sphere->setScale(this->scale);
	return sphere;
}

CylinderParser::CylinderParser(FileParser &file, GroupParser &parent)
: ObjectParser(file, parent)
, size(1)
{
	registerAttributeCallback("size", ParserAttributeCallback(&CylinderParser::parseSize));
}

void CylinderParser::parseSize(xmlAttr *attr)
{
	parseAttrFloat(attr, &this->size);
}

Object *CylinderParser::toObject()
{
	Cylinder *cylinder = new Cylinder(this->size);
	cylinder->setMaterial(this->file.getMaterial(this->material));
	cylinder->setPosition(this->position);
	cylinder->setRotation(this->rotation);
	cylinder->setScale(this->scale);
	return cylinder;
}

ConeParser::ConeParser(FileParser &file, GroupParser &parent)
: ObjectParser(file, parent)
, size(1)
{
	registerAttributeCallback("size", ParserAttributeCallback(&ConeParser::parseSize));
}

void ConeParser::parseSize(xmlAttr *attr)
{
	parseAttrFloat(attr, &this->size);
}

Object *ConeParser::toObject()
{
	Cone *cone = new Cone(this->size);
	cone->setMaterial(this->file.getMaterial(this->material));
	cone->setPosition(this->position);
	cone->setRotation(this->rotation);
	cone->setScale(this->scale);
	return cone;
}

PlaneParser::PlaneParser(FileParser &file, GroupParser &parent)
: ObjectParser(file, parent)
, normal(0, 1, 0)
{
	registerNodeCallback("Normal", ParserNodeCallback(&PlaneParser::parseNormal));
}

void PlaneParser::parseNormal(xmlNode *node)
{
	parseVec3(node, &this->normal);
}

Object *PlaneParser::toObject()
{
	Plane *plane = new Plane(this->normal);
	plane->setMaterial(this->file.getMaterial(this->material));
	plane->setPosition(this->position);
	plane->setRotation(this->rotation);
	plane->setScale(this->scale);
	return plane;
}

TriangleParser::TriangleParser(FileParser &file, GroupParser &parent)
: ObjectParser(file, parent)
{
}

Object *TriangleParser::toObject()
{
	Triangle *triangle = new Triangle();
	triangle->setMaterial(this->file.getMaterial(this->material));
	triangle->setPosition(this->position);
	triangle->setRotation(this->rotation);
	triangle->setScale(this->scale);
	return triangle;
}
