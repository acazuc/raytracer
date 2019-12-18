#include "FileParser.h"
#include "Filters/Color/Brightness.h"
#include "Filters/Color/Saturation.h"
#include "Filters/Color/GreyShade.h"
#include "Filters/Color/Negative.h"
#include "Filters/Color/Contrast.h"
#include "Filters/DepthOfField.h"
#include "Filters/Color/Sepia.h"
#include "Filters/Color/Gamma.h"
#include "Filters/Denoising.h"
#include "./MaterialParser.h"
#include "Filters/Fisheye.h"
#include "Filters/Sobel.h"
#include "./GroupParser.h"
#include "Filters/Fxaa.h"
#include "Filters/Blur.h"
#include "Filters/Glow.h"
#include "Filters/Cel.h"
#include "Raytracer.h"
#include "Material.h"
#include "Texture.h"
#include "Debug.h"
#include "PNG.h"
#include <algorithm>

FileParser::FileParser(std::string filename)
: filename(filename)
{
	this->defaultMaterial = new Material();
	this->group = new GroupParser(*this, nullptr);
	this->cameraPosition = Vec3(0);
	this->cameraRotation = Vec3(0);
	this->cameraFov = 66;
	this->threads = 1;
	this->samples = 1;
	this->shading = true;
	this->width = 640;
	this->height = 480;
	this->globalIlluminationSamples = 50;
	this->globalIlluminationFactor = 1;
	this->globalIllumination = false;
	this->maxReflection = 10;
}

Raytracer *FileParser::createRaytracer()
{
	Raytracer *raytracer = new Raytracer(this->width, this->height);
	raytracer->setGlobalIlluminationSamples(this->globalIlluminationSamples);
	raytracer->setGlobalIlluminationFactor(this->globalIlluminationFactor);
	raytracer->setGlobalIllumination(this->globalIllumination);
	raytracer->setThreads(this->threads);
	raytracer->setSamples(this->samples);
	raytracer->setShading(this->shading);
	raytracer->setCameraPosition(this->cameraPosition);
	raytracer->setCameraRotation(this->cameraRotation);
	raytracer->setCameraFov(this->cameraFov);
	this->filters.addFilters(raytracer);
	this->group->addMaterials(raytracer);
	this->group->addObjects(raytracer);
	this->group->addLights(raytracer);
	return raytracer;
}

void FileParser::parseFilters(xmlNode *node)
{
	this->filters.parseChilds(node);
}

void FileParser::parseCamera(xmlNode *node)
{
	for (xmlAttr *attr = node->properties; attr; attr = attr->next)
	{
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("fov"))
		{
			parseAttrFloat(attr, &this->cameraFov);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("width"))
		{
			parseAttrUInt(attr, &this->width);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("height"))
		{
			parseAttrUInt(attr, &this->height);
			continue;
		}
	}
	for (xmlNode *child = node->children; child; child = child->next)
	{
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Position"))
		{
			parseVec3(child, &this->cameraPosition);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Rotation"))
		{
			parseVec3(child, &this->cameraRotation);
			continue;
		}
	}
}

void FileParser::parseScene(xmlNode *node)
{
	for (xmlAttr *attr = node->properties; attr; attr = attr->next)
	{
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("samples"))
		{
			size_t samples;
			parseAttrUInt(attr, &samples);
			this->samples = samples;
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("threads"))
		{
			size_t threads;
			parseAttrUInt(attr, &threads);
			this->threads = threads;
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("shading"))
		{
			parseAttrBool(attr, &this->shading);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("globalIlluminationSamples"))
		{
			parseAttrUInt(attr, &this->globalIlluminationSamples);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("globalIlluminationFactor"))
		{
			parseAttrFloat(attr, &this->globalIlluminationFactor);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("globalIllumination"))
		{
			parseAttrBool(attr, &this->globalIllumination);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("maxReflection"))
		{
			parseAttrUInt(attr, &this->maxReflection);
			continue;
		}
	}
	for (xmlNode *child = node->children; child; child = child->next)
	{
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Camera"))
		{
			parseCamera(child);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Filters"))
		{
			parseFilters(child);
			continue;
		}
		if (this->group->parseChild(child))
			continue;
	}
}

bool FileParser::parse()
{
	xmlDoc *doc = xmlReadFile(this->filename.c_str(), "utf8", 0);
	if (!doc)
	{
		std::cerr << "Failed to parse XML file: " << this->filename << std::endl;
		return false;
	}
	xmlNode *root = xmlDocGetRootElement(doc);
	if (!root)
	{
		std::cerr << "Failed to get XML root node in " << this->filename << std::endl;
		return false;
	}
	if (root->type != XML_ELEMENT_NODE)
	{
		std::cerr << "Root node isn't a node in " << this->filename << std::endl;
		return false;
	}
	if (std::string(reinterpret_cast<const char*>(root->name)).compare("Scene"))
	{
		std::cerr << "Root node isn't a <Scene> node in " << this->filename << std::endl;
		return false;
	}
	parseScene(root);
	return true;
}

void FileParser::registerMaterial(std::string name, Material *material)
{
	this->materials.emplace(name, material);
}

Material *FileParser::getMaterial(std::string name)
{
	std::unordered_map<std::string, Material*>::iterator iter = this->materials.find(name);
	if (iter == this->materials.end())
		return this->defaultMaterial;
	return iter->second;
}
