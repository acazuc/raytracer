#include "ModelParser.h"

ModelParser::ModelParser(FileParser &file)
: file(file)
, position(0)
, rotation(0)
, scale(0)
{
	registerNodeCallback("Position", ParserNodeCallback(&ModelParser::parsePosition));
	registerNodeCallback("Rotation", ParserNodeCallback(&ModelParser::parseRotation));
	registerNodeCallback("Scale", ParserNodeCallback(&ModelParser::parseScale));
}

void ModelParser::parsePosition(xmlNode *node)
{
	parseVec3(node, &this->position);
}

void ModelParser::parseRotation(xmlNode *node)
{
	parseVec3(node, &this->rotation);
}

void ModelParser::parseScale(xmlNode *node)
{
	parseVec3(node, &this->scale);
}
