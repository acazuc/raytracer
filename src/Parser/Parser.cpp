#include "Parser.h"
#include "Texture.h"
#include "Verbose.h"
#include "PNG.h"

Parser::Parser()
{
}

Parser::~Parser()
{
}

void Parser::registerAttributeCallback(std::string name, ParserAttributeCallback callback)
{
	this->attributesCallbacks.emplace(name, callback);
}

void Parser::registerNodeCallback(std::string name, ParserNodeCallback callback)
{
	this->nodesCallbacks.emplace(name, callback);
}

void Parser::parseAttributes(xmlNode *node)
{
	for (xmlAttr *attr = node->properties; attr; attr = attr->next)
	{
		if (parseAttribute(attr))
			goto nextAttr;
		VERBOSE_ERROR("Unknown <" << node->name << "> attribute: " << attr->name);
nextAttr:
		continue;
	}
}

bool Parser::parseAttribute(xmlAttr *attr)
{
	std::string attrName(reinterpret_cast<const char*>(attr->name));
	for (std::unordered_map<std::string, ParserAttributeCallback>::iterator iter = this->attributesCallbacks.begin(); iter != this->attributesCallbacks.end(); ++iter)
	{
		if (!attrName.compare(iter->first))
		{
			(this->*(iter->second))(attr);
			return true;
		}
	}
	return false;
}

void Parser::parseChilds(xmlNode *node)
{
	for (xmlNode *child = node->children; child; child = child->next)
	{
		if (parseChild(child))
			goto nextChild;
		if (child->type == XML_TEXT_NODE || child->type == XML_COMMENT_NODE)
			goto nextChild;
		VERBOSE_ERROR("Unknown <" << node->name << "> child: <" << child->name << ">");
nextChild:
		continue;
	}
}

bool Parser::parseChild(xmlNode *child)
{
	std::string childName(reinterpret_cast<const char*>(child->name));
	for (std::unordered_map<std::string, ParserNodeCallback>::iterator iter = this->nodesCallbacks.begin(); iter != this->nodesCallbacks.end(); ++iter)
	{
		if (!childName.compare(iter->first))
		{
			(this->*(iter->second))(child);
			return true;
		}
	}
	return false;
}

void Parser::parse(xmlNode *node)
{
	parseAttributes(node);
	parseChilds(node);
}

bool Parser::parseAttrUInt(xmlAttr *attr, uint64_t *value)
{
	if (!attr->children)
		return false;
	try
	{
		*value = std::stoull(std::string(reinterpret_cast<const char*>(attr->children->content)));
	}
	catch (std::exception &e)
	{
		return false;
	}
	return true;
}

bool Parser::parseAttrInt(xmlAttr *attr, int64_t *value)
{
	if (!attr->children)
		return false;
	try
	{
		*value = std::stoll(std::string(reinterpret_cast<const char*>(attr->children->content)));
	}
	catch (std::exception &e)
	{
		return false;
	}
	return true;
}

bool Parser::parseAttrFloat(xmlAttr *attr, float *value)
{
	if (!attr->children)
		return false;
	try
	{
		*value = std::stof(std::string(reinterpret_cast<const char*>(attr->children->content)));
	}
	catch (std::exception &e)
	{
		return false;
	}
	return true;
}

bool Parser::parseAttrBool(xmlAttr *attr, bool *value)
{
	if (!attr->children)
		return false;
	if (!std::string(reinterpret_cast<const char*>(attr->children->content)).compare("true"))
	{
		*value = true;
		return true;
	}
	if (!std::string(reinterpret_cast<const char*>(attr->children->content)).compare("false"))
	{
		*value = false;
		return true;
	}
	return false;
}

bool Parser::parseAttrString(xmlAttr *attr, std::string *value)
{
	if (!attr->children)
		return false;
	*value = std::string(reinterpret_cast<const char*>(attr->children->content));
	return true;
}

void Parser::parseVec4(xmlNode *node, Vec4 *vec)
{
	for (xmlAttr *attr = node->properties; attr; attr = attr->next)
	{
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("x")
				|| !std::string(reinterpret_cast<const char*>(attr->name)).compare("r")
				|| !std::string(reinterpret_cast<const char*>(attr->name)).compare("red"))
		{
			parseAttrFloat(attr, &vec->x);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("y")
				|| !std::string(reinterpret_cast<const char*>(attr->name)).compare("g")
				|| !std::string(reinterpret_cast<const char*>(attr->name)).compare("green"))
		{
			parseAttrFloat(attr, &vec->y);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("z")
				|| !std::string(reinterpret_cast<const char*>(attr->name)).compare("b")
				|| !std::string(reinterpret_cast<const char*>(attr->name)).compare("blue"))
		{
			parseAttrFloat(attr, &vec->z);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("w")
				|| !std::string(reinterpret_cast<const char*>(attr->name)).compare("a")
				|| !std::string(reinterpret_cast<const char*>(attr->name)).compare("alpha"))
		{
			parseAttrFloat(attr, &vec->a);
			continue;
		}
	}
}

void Parser::parseVec3(xmlNode *node, Vec3 *vec)
{
	for (xmlAttr *attr = node->properties; attr; attr = attr->next)
	{
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("x")
				|| !std::string(reinterpret_cast<const char*>(attr->name)).compare("r")
				|| !std::string(reinterpret_cast<const char*>(attr->name)).compare("red"))
		{
			parseAttrFloat(attr, &vec->x);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("y")
				|| !std::string(reinterpret_cast<const char*>(attr->name)).compare("g")
				|| !std::string(reinterpret_cast<const char*>(attr->name)).compare("green"))
		{
			parseAttrFloat(attr, &vec->y);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("z")
				|| !std::string(reinterpret_cast<const char*>(attr->name)).compare("b")
				|| !std::string(reinterpret_cast<const char*>(attr->name)).compare("blue"))
		{
			parseAttrFloat(attr, &vec->z);
			continue;
		}
	}
}

void Parser::parseVec2(xmlNode *node, Vec2 *vec)
{
	for (xmlAttr *attr = node->properties; attr; attr = attr->next)
	{
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("x")
				|| !std::string(reinterpret_cast<const char*>(attr->name)).compare("u"))
		{
			parseAttrFloat(attr, &vec->x);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("y")
				|| !std::string(reinterpret_cast<const char*>(attr->name)).compare("u"))
		{
			parseAttrFloat(attr, &vec->y);
			continue;
		}
	}
}

void Parser::parseVec1(xmlNode *node, float *val)
{
	for (xmlAttr *attr = node->properties; attr; attr = attr->next)
	{
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("value"))
		{
			parseAttrFloat(attr, val);
			continue;
		}
	}
}

void Parser::parseTexture(xmlNode *node, Texture **texture)
{
	delete (*texture);
	*texture = new Texture();
	for (xmlAttr *attr = node->properties; attr; attr = attr->next)
	{
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("file"))
		{
			std::string file;
			parseAttrString(attr, &file);
			uint8_t *data;
			uint32_t width;
			uint32_t height;
			if (!PNG::read(file, data, width, height))
				continue;
			(*texture)->setData(width, height, data);
			delete[] (data);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("filtering"))
		{
			std::string filtering;
			parseAttrString(attr, &filtering);
			if (!filtering.compare("nearest"))
				(*texture)->setFiltering(TEXTURE_FILTERING_NEAREST);
			else if (!filtering.compare("linear"))
				(*texture)->setFiltering(TEXTURE_FILTERING_LINEAR);
			else if (!filtering.compare("cubic"))
				(*texture)->setFiltering(TEXTURE_FILTERING_CUBIC);
			else
				VERBOSE_WARN("Unknown texture filtering: " << filtering);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("wrapS"))
		{
			std::string wrap;
			parseAttrString(attr, &wrap);
			if (!wrap.compare("clamp"))
				(*texture)->setWrapS(TEXTURE_WRAP_CLAMP);
			else if (!wrap.compare("repeat"))
				(*texture)->setWrapS(TEXTURE_WRAP_REPEAT);
			else if (!wrap.compare("mirrored_repeat"))
				(*texture)->setWrapS(TEXTURE_WRAP_MIRRORED_REPEAT);
			else
				VERBOSE_WARN("Unknown texture wrap: " << wrap);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("wrapT"))
		{
			std::string wrap;
			parseAttrString(attr, &wrap);
			if (!wrap.compare("clamp"))
				(*texture)->setWrapT(TEXTURE_WRAP_CLAMP);
			else if (!wrap.compare("repeat"))
				(*texture)->setWrapT(TEXTURE_WRAP_REPEAT);
			else if (!wrap.compare("mirrored_repeat"))
				(*texture)->setWrapT(TEXTURE_WRAP_MIRRORED_REPEAT);
			else
				VERBOSE_WARN("Unknown texture wrap: " << wrap);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("wrap"))
		{
			std::string wrap;
			parseAttrString(attr, &wrap);
			if (!wrap.compare("clamp"))
				(*texture)->setWrap(TEXTURE_WRAP_CLAMP);
			else if (!wrap.compare("repeat"))
				(*texture)->setWrap(TEXTURE_WRAP_REPEAT);
			else if (!wrap.compare("mirrored_repeat"))
				(*texture)->setWrap(TEXTURE_WRAP_MIRRORED_REPEAT);
			else
				VERBOSE_WARN("Unknown texture wrap: " << wrap);
			continue;
		}
	}
}

