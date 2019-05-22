#include "MaterialParser.h"
#include "Material.h"

MaterialParser::MaterialParser(FileParser &file)
: file(file)
, emissiveTexture(nullptr)
, specularTexture(nullptr)
, diffuseTexture(nullptr)
, normalTexture(nullptr)
, emissiveColor(0)
, specularColor(0)
, diffuseColor(.5)
, specularFactor(100)
, reflection(0)
, refraction(1)
, opacity(1)
{
	registerAttributeCallback("name", ParserAttributeCallback(&MaterialParser::parseName));
	registerAttributeCallback("reflection", ParserAttributeCallback(&MaterialParser::parseReflection));
	registerAttributeCallback("refraction", ParserAttributeCallback(&MaterialParser::parseRefraction));
	registerAttributeCallback("transparency", ParserAttributeCallback(&MaterialParser::parseTransparency));
	registerAttributeCallback("opacity", ParserAttributeCallback(&MaterialParser::parseOpacity));
	registerNodeCallback("EmissiveTexture", ParserNodeCallback(&MaterialParser::parseEmissiveTexture));
	registerNodeCallback("SpecularTexture", ParserNodeCallback(&MaterialParser::parseSpecularTexture));
	registerNodeCallback("DiffuseTexture", ParserNodeCallback(&MaterialParser::parseDiffuseTexture));
	registerNodeCallback("NormalTexture", ParserNodeCallback(&MaterialParser::parseNormalTexture));
	registerNodeCallback("EmissiveColor", ParserNodeCallback(&MaterialParser::parseEmissiveColor));
	registerNodeCallback("SpecularColor", ParserNodeCallback(&MaterialParser::parseSpecularColor));
	registerNodeCallback("DiffuseColor", ParserNodeCallback(&MaterialParser::parseDiffuseColor));
	registerNodeCallback("SpecularFactor", ParserNodeCallback(&MaterialParser::parseSpecularFactor));
}

void MaterialParser::parseName(xmlAttr *attr)
{
	parseAttrString(attr, &this->name);
}

void MaterialParser::parseReflection(xmlAttr *attr)
{
	parseAttrFloat(attr, &this->reflection);
}

void MaterialParser::parseRefraction(xmlAttr *attr)
{
	parseAttrFloat(attr, &this->refraction);
}

void MaterialParser::parseTransparency(xmlAttr *attr)
{
	float transparency;
	parseAttrFloat(attr, &transparency);
	this->opacity = 1 - transparency;
}

void MaterialParser::parseOpacity(xmlAttr *attr)
{
	parseAttrFloat(attr, &this->opacity);
}

void MaterialParser::parseEmissiveTexture(xmlNode *node)
{
	parseTexture(node, &this->emissiveTexture);
}

void MaterialParser::parseSpecularTexture(xmlNode *node)
{
	parseTexture(node, &this->specularTexture);
}

void MaterialParser::parseDiffuseTexture(xmlNode *node)
{
	parseTexture(node, &this->diffuseTexture);
}

void MaterialParser::parseNormalTexture(xmlNode *node)
{
	parseTexture(node, &this->normalTexture);
}

void MaterialParser::parseEmissiveColor(xmlNode *node)
{
	parseVec3(node, &this->emissiveColor);
}

void MaterialParser::parseSpecularColor(xmlNode *node)
{
	parseVec3(node, &this->specularColor);
}

void MaterialParser::parseDiffuseColor(xmlNode *node)
{
	parseVec3(node, &this->diffuseColor);
}

void MaterialParser::parseSpecularFactor(xmlNode *node)
{
	parseVec1(node, &this->specularFactor);
}

Material *MaterialParser::toMaterial()
{
	Material *material = new Material();
	material->emissiveTexture = this->emissiveTexture;
	material->specularTexture = this->specularTexture;
	material->diffuseTexture = this->diffuseTexture;
	material->normalTexture = this->normalTexture;
	material->emissiveColor = this->emissiveColor;
	material->specularColor = this->specularColor;
	material->diffuseColor = this->diffuseColor;
	material->specularFactor = this->specularFactor;
	material->reflection = this->reflection;
	material->refraction = this->refraction;
	material->opacity = this->opacity;
	return material;
}
