#include "LightsParser.h"
#include "Lights/DirectionalLight.h"
#include "Lights/PunctualLight.h"
#include "Raytracer.h"

LightParser::LightParser(FileParser &file, GroupParser &parent)
: parent(parent)
, file(file)
, color(1)
, intensity(1)
{
	registerAttributeCallback("power", ParserAttributeCallback(&LightParser::parseIntensity));
	registerNodeCallback("Color", ParserNodeCallback(&LightParser::parseColor));
}

void LightParser::parseColor(xmlNode *node)
{
	parseVec3(node, &this->color);
}

void LightParser::parseIntensity(xmlAttr *attr)
{
	parseAttrFloat(attr, &this->intensity);
}

PunctualLightParser::PunctualLightParser(FileParser &file, GroupParser &parent)
: LightParser(file, parent)
, position(0)
{
	registerNodeCallback("Position", ParserNodeCallback(&PunctualLightParser::parsePosition));
}

void PunctualLightParser::parsePosition(xmlNode *node)
{
	parseVec3(node, &this->position);
}

void PunctualLightParser::addToRaytracer(Raytracer *raytracer)
{
	PunctualLight *light = new PunctualLight();
	light->setColor(this->color * this->intensity);
	light->setPosition(this->position);
	raytracer->addLight(light);
}

DirectionalLightParser::DirectionalLightParser(FileParser &file, GroupParser &parent)
: LightParser(file, parent)
, direction(0)
{
	registerNodeCallback("Direction", ParserNodeCallback(&DirectionalLightParser::parseDirection));
}

void DirectionalLightParser::parseDirection(xmlNode *node)
{
	parseVec3(node, &this->direction);
}

void DirectionalLightParser::addToRaytracer(Raytracer *raytracer)
{
	DirectionalLight *light = new DirectionalLight();
	light->setColor(this->color * this->intensity);
	light->setDirection(this->direction);
	raytracer->addLight(light);
}
