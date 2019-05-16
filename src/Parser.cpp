#include "Parser.h"
#include "Lights/DirectionalLight.h"
#include "Filters/Color/GreyShade.h"
#include "Filters/Color/Negative.h"
#include "Lights/PunctualLight.h"
#include "Filters/Color/Sepia.h"
#include "Objects/Cylinder.h"
#include "Objects/Triangle.h"
#include "Filters/Fisheye.h"
#include "Objects/Sphere.h"
#include "Filters/Sobel.h"
#include "Objects/Plane.h"
#include "Objects/Cone.h"
#include "Lights/Light.h"
#include "Filters/Fxaa.h"
#include "Filters/Blur.h"
#include "Filters/Cel.h"
#include "Raytracer.h"
#include "Material.h"
#include "Image.h"
#include "PNG.h"
#include <algorithm>
#include <iostream>

Parser::Parser(std::string filename)
: filename(filename)
{
	this->cameraPosition = Vec3(0);
	this->cameraRotation = Vec3(0);
	this->cameraFov = 66;
	this->ambient = Vec3(0);
	this->threads = 1;
	this->samples = 1;
	this->shading = true;
	this->width = 640;
	this->height = 480;
	this->defaultMaterial = new Material();
	this->globalIllumination = false;
	this->ambientOcclusion = false;
	this->celShadingCels = 0xff;
	this->fisheyeAperture = 178;
	this->blurRadius = 1;
	this->celShading = false;
	this->greyShade = false;
	this->negative = false;
	this->fisheye = false;
	this->sobel = false;
	this->sepia = false;
	this->fxaa = false;
	this->blur = false;
}

Raytracer *Parser::createRaytracer()
{
	Raytracer *raytracer = new Raytracer(this->width, this->height);
	raytracer->setGlobalIllumination(this->globalIllumination);
	raytracer->setAmbientOcclusion(this->ambientOcclusion);
	raytracer->setThreads(this->threads);
	raytracer->setSamples(this->samples);
	raytracer->setShading(this->shading);
	raytracer->setFov(this->cameraFov);
	raytracer->setAmbient(this->ambient);
	raytracer->setPos(this->cameraPosition);
	raytracer->setRot(this->cameraRotation);
	for (size_t i = 0; i < this->objects.size(); ++i)
		raytracer->addObject(this->objects[i]);
	for (size_t i = 0; i < this->lights.size(); ++i)
		raytracer->addLight(this->lights[i]);
	if (this->sobel)
		raytracer->addFilter(new Sobel());
	if (this->greyShade)
		raytracer->addFilter(new GreyShade());
	if (this->fisheye)
		raytracer->addFilter(new Fisheye(this->fisheyeAperture));
	if (this->negative)
		raytracer->addFilter(new Negative());
	if (this->sepia)
		raytracer->addFilter(new Sepia());
	if (this->celShading)
		raytracer->addFilter(new Cel(this->celShadingCels));
	if (this->fxaa)
		raytracer->addFilter(new Fxaa());
	if (this->blur)
		raytracer->addFilter(new Blur(this->blurRadius));
	return raytracer;
}

void Parser::parsePunctualLight(xmlNode *node)
{
	PunctualLight *light = new PunctualLight();
	for (xmlAttr *attr = node->properties; attr; attr = attr->next)
	{
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("power"))
		{
			float power;
			parseAttrFloat(attr, &power);
			light->setIntensity(power);
		}
	}
	for (xmlNode *child = node->children; child; child = child->next)
	{
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Position"))
		{
			Vec3 position;
			parseVec3(child, &position);
			light->setPosition(position);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Color"))
		{
			Vec3 color;
			parseVec3(child, &color);
			light->setColor(color);
			continue;
		}
	}
	this->lights.push_back(light);
}

void Parser::parseDirectionalLight(xmlNode *node)
{
	DirectionalLight *light = new DirectionalLight();
	for (xmlAttr *attr = node->properties; attr; attr = attr->next)
	{
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("power"))
		{
			float power;
			parseAttrFloat(attr, &power);
			light->setIntensity(power);
		}
	}
	for (xmlNode *child = node->children; child; child = child->next)
	{
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Direction"))
		{
			Vec3 direction;
			parseVec3(child, &direction);
			light->setDirection(direction);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Color"))
		{
			Vec3 color;
			parseVec3(child, &color);
			light->setColor(color);
			continue;
		}
	}
	this->lights.push_back(light);
}

void Parser::parseLights(xmlNode *node)
{
	for (xmlNode *child = node->children; child; child = child->next)
	{
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("PunctualLight"))
		{
			parsePunctualLight(child);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("DirectionalLight"))
		{
			parseDirectionalLight(child);
			continue;
		}
	}
}

void Parser::parseSphere(xmlNode *node)
{
	Sphere *sphere = new Sphere();
	std::string material;
	for (xmlAttr *attr = node->properties; attr; attr = attr->next)
	{
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("material"))
		{
			parseAttrString(attr, &material);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("size"))
		{
			float size;
			parseAttrFloat(attr, &size);
			sphere->setSize(size);
			continue;
		}
	}
	sphere->setMaterial(getMaterial(material));
	for (xmlNode *child = node->children; child; child = child->next)
	{
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Position"))
		{
			Vec3 position;
			parseVec3(child, &position);
			sphere->setPosition(position);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Rotation"))
		{
			Vec3 rotation;
			parseVec3(child, &rotation);
			sphere->setRotation(rotation);
			continue;
		}
	}
	this->objects.push_back(sphere);
}

void Parser::parseCylinder(xmlNode *node)
{
	Cylinder *cylinder = new Cylinder();
	std::string material;
	for (xmlAttr *attr = node->properties; attr; attr = attr->next)
	{
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("material"))
		{
			parseAttrString(attr, &material);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("size"))
		{
			float size;
			parseAttrFloat(attr, &size);
			cylinder->setSize(size);
			continue;
		}
	}
	cylinder->setMaterial(getMaterial(material));
	for (xmlNode *child = node->children; child; child = child->next)
	{
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Position"))
		{
			Vec3 position;
			parseVec3(child, &position);
			cylinder->setPosition(position);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Rotation"))
		{
			Vec3 rotation;
			parseVec3(child, &rotation);
			cylinder->setRotation(rotation);
			continue;
		}
	}
	this->objects.push_back(cylinder);
}

void Parser::parseCone(xmlNode *node)
{
	Cone *cone = new Cone();
	std::string material;
	for (xmlAttr *attr = node->properties; attr; attr = attr->next)
	{
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("material"))
		{
			parseAttrString(attr, &material);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("size"))
		{
			float size;
			parseAttrFloat(attr, &size);
			cone->setSize(size);
			continue;
		}
	}
	cone->setMaterial(getMaterial(material));
	for (xmlNode *child = node->children; child; child = child->next)
	{
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Position"))
		{
			Vec3 position;
			parseVec3(child, &position);
			cone->setPosition(position);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Rotation"))
		{
			Vec3 rotation;
			parseVec3(child, &rotation);
			cone->setRotation(rotation);
			continue;
		}
	}
	this->objects.push_back(cone);
}

void Parser::parsePlane(xmlNode *node)
{
	Plane *plane = new Plane();
	std::string material;
	for (xmlAttr *attr = node->properties; attr; attr = attr->next)
	{
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("material"))
		{
			parseAttrString(attr, &material);
			continue;
		}
	}
	plane->setMaterial(getMaterial(material));
	for (xmlNode *child = node->children; child; child = child->next)
	{
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Position"))
		{
			Vec3 position;
			parseVec3(child, &position);
			plane->setPosition(position);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Rotation"))
		{
			Vec3 rotation;
			parseVec3(child, &rotation);
			plane->setRotation(rotation);
			continue;
		}
	}
	this->objects.push_back(plane);
}

void Parser::parseTriangle(xmlNode *node)
{
	(void)node;
}

void Parser::parseObjects(xmlNode *node)
{
	for (xmlNode *child = node->children; child; child = child->next)
	{
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Sphere"))
		{
			parseSphere(child);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Cone"))
		{
			parseCone(child);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Cylinder"))
		{
			parseCylinder(child);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Plane"))
		{
			parsePlane(child);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Triangle"))
		{
			parseTriangle(child);
			continue;
		}
	}
}

void Parser::parseMaterial(xmlNode *node)
{
	Material *material = new Material();
	std::string name;
	for (xmlAttr *attr = node->properties; attr; attr = attr->next)
	{
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("name"))
		{
			parseAttrString(attr, &name);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("transparency"))
		{
			float transparency;
			parseAttrFloat(attr, &transparency);
			transparency = std::clamp(transparency, 0.f, 1.f);
			material->opacity = 1 - transparency;
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("refraction"))
		{
			parseAttrFloat(attr, &material->refraction);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("reflection"))
		{
			parseAttrFloat(attr, &material->reflection);
			material->reflection = std::clamp(material->reflection, 0.f, 1.f);
			continue;
		}
	}
	if (!this->materials.emplace(name, material).second)
	{
		delete (material);
		std::cerr << "Material with name " << name << " already exists" << std::endl;
		return;
	}
	for (xmlNode *child = node->children; child; child = child->next)
	{
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("DiffuseColor"))
		{
			parseVec3(child, &material->diffuseColor);
			material->diffuseColor = clamp(material->diffuseColor, 0.f, 1.f);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("DiffuseTexture"))
		{
			parseTexture(child, &material->diffuseTexture);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("SpecularColor"))
		{
			parseVec3(child, &material->specularColor);
			material->specularColor = clamp(material->specularColor, 0.f, 1.f);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("SpecularFactor"))
		{
			parseVec1(child, &material->specularFactor);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("AmbientColor"))
		{
			parseVec3(child, &material->ambientColor);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("NormalTexture"))
		{
			parseTexture(child, &material->normalTexture);
			continue;
		}
	}
}

void Parser::parseMaterials(xmlNode *node)
{
	for (xmlNode *child = node->children; child; child = child->next)
	{
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Material"))
		{
			parseMaterial(child);
			continue;
		}
	}
}

void Parser::parseSobel(xmlNode *node)
{
	this->sobel = true;
	(void)node;
}

void Parser::parseGreyShade(xmlNode *node)
{
	this->greyShade = true;
	(void)node;
}

void Parser::parseCelShading(xmlNode *node)
{
	this->celShading = true;
	for (xmlAttr *attr = node->properties; attr; attr = attr->next)
	{
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("cels"))
		{
			uint64_t value;
			parseAttrUInt(attr, &value);
			this->celShadingCels = value;
			continue;
		}
	}
}

void Parser::parseFxaa(xmlNode *node)
{
	this->fxaa = true;
	(void)node;
}

void Parser::parseNegative(xmlNode *node)
{
	this->negative = true;
	(void)node;
}

void Parser::parseSepia(xmlNode *node)
{
	this->sepia = true;
	(void)node;
}

void Parser::parseBlur(xmlNode *node)
{
	this->blur = true;
	for (xmlAttr *attr = node->properties; attr; attr = attr->next)
	{
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("radius"))
		{
			uint64_t value;
			parseAttrUInt(attr, &value);
			this->blurRadius = value;
			continue;
		}
	}
	(void)node;
}

void Parser::parseFisheye(xmlNode *node)
{
	this->blur = true;
	for (xmlAttr *attr = node->properties; attr; attr = attr->next)
	{
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("aperture"))
		{
			parseAttrFloat(attr, &this->fisheyeAperture);
			continue;
		}
	}
	(void)node;
}

void Parser::parseFilters(xmlNode *node)
{
	for (xmlNode *child = node->children; child; child = child->next)
	{
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Sobel"))
		{
			parseSobel(child);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("GreyShade"))
		{
			parseGreyShade(child);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("CelShading"))
		{
			parseCelShading(child);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Fxaa"))
		{
			parseFxaa(child);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Negative"))
		{
			parseNegative(child);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Sepia"))
		{
			parseSepia(child);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Blur"))
		{
			parseBlur(child);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Fisheye"))
		{
			parseFisheye(child);
			continue;
		}
	}
}

void Parser::parseCamera(xmlNode *node)
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

void Parser::parseEnvironment(xmlNode *node)
{
	for (xmlNode *child = node->children; child; child = child->next)
	{
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Ambient"))
		{
			parseVec3(child, &this->ambient);
			continue;
		}
	}
}

void Parser::parseScene(xmlNode *node)
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
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("globalIllumination"))
		{
			parseAttrBool(attr, &this->globalIllumination);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("ambientOcclusion"))
		{
			parseAttrBool(attr, &this->ambientOcclusion);
			continue;
		}
	}
	for (xmlNode *child = node->children; child; child = child->next)
	{
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Environment"))
		{
			parseEnvironment(child);
			continue;
		}
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
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Materials"))
		{
			parseMaterials(child);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Objects"))
		{
			parseObjects(child);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Lights"))
		{
			parseLights(child);
			continue;
		}
	}
}

bool Parser::parse()
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

void Parser::parseVec1(xmlNode *node, float *val)
{
	for (xmlAttr *attr = node->properties; attr; attr = attr->next)
	{
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("x")
				|| !std::string(reinterpret_cast<const char*>(attr->name)).compare("value")
				|| !std::string(reinterpret_cast<const char*>(attr->name)).compare("r")
				|| !std::string(reinterpret_cast<const char*>(attr->name)).compare("red"))
		{
			parseAttrFloat(attr, val);
			continue;
		}
	}
}

void Parser::parseTexture(xmlNode *node, Image **texture)
{
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
				return;
			*texture = new Image();
			(*texture)->setData(width, height, data);
			return;
		}
	}
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

Material *Parser::getMaterial(std::string name)
{
	std::unordered_map<std::string, Material*>::iterator iter = this->materials.find(name);
	if (iter == this->materials.end())
		return this->defaultMaterial;
	return iter->second;
}
