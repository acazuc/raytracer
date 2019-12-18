#include "ObjectsParser.h"
#include "Objects/Triangle.h"
#include "Objects/Cylinder.h"
#include "Objects/Sphere.h"
#include "Objects/Plane.h"
#include "Objects/Cone.h"
#include "./FileParser.h"
#include "Raytracer.h"

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

void SphereParser::addToRaytracer(Raytracer *raytracer)
{
	Sphere *sphere = new Sphere(this->size);
	sphere->setMaterial(this->file.getMaterial(this->material));
	sphere->setPosition(this->position);
	sphere->setRotation(this->rotation);
	sphere->setScale(this->scale);
	raytracer->addObject(sphere);
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

void CylinderParser::addToRaytracer(Raytracer *raytracer)
{
	Cylinder *cylinder = new Cylinder(this->size);
	cylinder->setMaterial(this->file.getMaterial(this->material));
	cylinder->setPosition(this->position);
	cylinder->setRotation(this->rotation);
	cylinder->setScale(this->scale);
	raytracer->addObject(cylinder);
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

void ConeParser::addToRaytracer(Raytracer *raytracer)
{
	Cone *cone = new Cone(this->size);
	cone->setMaterial(this->file.getMaterial(this->material));
	cone->setPosition(this->position);
	cone->setRotation(this->rotation);
	cone->setScale(this->scale);
	raytracer->addObject(cone);
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

void PlaneParser::addToRaytracer(Raytracer *raytracer)
{
	Plane *plane = new Plane(this->normal);
	plane->setMaterial(this->file.getMaterial(this->material));
	plane->setPosition(this->position);
	plane->setRotation(this->rotation);
	plane->setScale(this->scale);
	raytracer->addObject(plane);
}

TriangleParser::TriangleParser(FileParser &file, GroupParser &parent)
: ObjectParser(file, parent)
, point1(0)
, point2(0)
, point3(0)
, norm1(0)
, norm2(0)
, norm3(0)
, UV1(0)
, UV2(0)
, UV3(0)
{
	registerNodeCallback("Point1", ParserNodeCallback(&TriangleParser::parsePoint1));
	registerNodeCallback("Point2", ParserNodeCallback(&TriangleParser::parsePoint2));
	registerNodeCallback("Point3", ParserNodeCallback(&TriangleParser::parsePoint3));
	registerNodeCallback("Norm1", ParserNodeCallback(&TriangleParser::parseNorm1));
	registerNodeCallback("Norm2", ParserNodeCallback(&TriangleParser::parseNorm2));
	registerNodeCallback("Norm3", ParserNodeCallback(&TriangleParser::parseNorm3));
	registerNodeCallback("UV1", ParserNodeCallback(&TriangleParser::parseUV1));
	registerNodeCallback("UV2", ParserNodeCallback(&TriangleParser::parseUV2));
	registerNodeCallback("UV3", ParserNodeCallback(&TriangleParser::parseUV3));
}

void TriangleParser::parsePoint1(xmlNode *node)
{
	parseVec3(node, &this->point1);
}

void TriangleParser::parsePoint2(xmlNode *node)
{
	parseVec3(node, &this->point2);
}

void TriangleParser::parsePoint3(xmlNode *node)
{
	parseVec3(node, &this->point3);
}

void TriangleParser::parseNorm1(xmlNode *node)
{
	parseVec3(node, &this->norm1);
}

void TriangleParser::parseNorm2(xmlNode *node)
{
	parseVec3(node, &this->norm2);
}

void TriangleParser::parseNorm3(xmlNode *node)
{
	parseVec3(node, &this->norm3);
}

void TriangleParser::parseUV1(xmlNode *node)
{
	parseVec2(node, &this->UV1);
}

void TriangleParser::parseUV2(xmlNode *node)
{
	parseVec2(node, &this->UV2);
}

void TriangleParser::parseUV3(xmlNode *node)
{
	parseVec2(node, &this->UV3);
}

void TriangleParser::addToRaytracer(Raytracer *raytracer)
{
	Triangle *triangle = new Triangle();
	triangle->setMaterial(this->file.getMaterial(this->material));
	triangle->setPosition(this->position);
	triangle->setRotation(this->rotation);
	triangle->setScale(this->scale);
	triangle->setPoint1(this->point1);
	triangle->setPoint2(this->point2);
	triangle->setPoint3(this->point3);
	triangle->setNorm1(this->norm1);
	triangle->setNorm2(this->norm2);
	triangle->setNorm3(this->norm3);
	triangle->setUV1(this->UV1);
	triangle->setUV2(this->UV2);
	triangle->setUV3(this->UV3);
	raytracer->addObject(triangle);
}
