#include "Parser.h"
#include "Filters/Color/Brightness.h"
#include "Filters/Color/Saturation.h"
#include "Lights/DirectionalLight.h"
#include "Filters/Color/GreyShade.h"
#include "Filters/Color/Negative.h"
#include "Filters/Color/Contrast.h"
#include "Lights/PunctualLight.h"
#include "Filters/DepthOfField.h"
#include "Filters/Color/Sepia.h"
#include "Filters/Color/Gamma.h"
#include "Filters/Denoising.h"
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
#include "Filters/Glow.h"
#include "Filters/Cel.h"
#include "Raytracer.h"
#include "Material.h"
#include "Image.h"
#include "Debug.h"
#include "PNG.h"
#include <algorithm>

Parser::Parser(std::string filename)
: filename(filename)
{
	this->cameraPosition = Vec3(0);
	this->cameraRotation = Vec3(0);
	this->cameraFov = 66;
	this->threads = 1;
	this->samples = 1;
	this->shading = true;
	this->width = 640;
	this->height = 480;
	this->defaultMaterial = new Material();
	this->denoisingThreshold = 0.999;
	this->denoisingRadius = 2;
	this->globalIlluminationDistance = 1000;
	this->globalIlluminationSamples = 50;
	this->globalIlluminationFactor = 1;
	this->globalIllumination = false;
	this->ambientOcclusionDistance = 10;
	this->ambientOcclusionSamples = 50;
	this->ambientOcclusionFactor = 1;
	this->ambientOcclusion = false;
	this->celShadingCels = 0xff;
	this->fisheyeAperture = 178;
	this->glowThreshold = 0.8;
	this->glowIntensity = 1;
	this->glowRadius = 10;
	this->blurRadius = 1;
	this->depthOfFieldAperture = 555;
	this->depthOfFieldFocal = 1;
	this->brightnessValue = 0;
	this->saturationValue = 1;
	this->contrastValue = 1;
	this->gammaValue = 1;
	this->depthOfField = false;
	this->celShading = false;
	this->brightness = false;
	this->saturation = false;
	this->greyShade = false;
	this->denoising = false;
	this->negative = false;
	this->contrast = false;
	this->fisheye = false;
	this->sobel = false;
	this->sepia = false;
	this->gamma = false;
	this->fxaa = false;
	this->blur = false;
	this->glow = false;
}

Raytracer *Parser::createRaytracer()
{
	Raytracer *raytracer = new Raytracer(this->width, this->height);
	raytracer->setGlobalIlluminationDistance(this->globalIlluminationDistance);
	raytracer->setGlobalIlluminationSamples(this->globalIlluminationSamples);
	raytracer->setGlobalIlluminationFactor(this->globalIlluminationFactor);
	raytracer->setGlobalIllumination(this->globalIllumination);
	raytracer->setAmbientOcclusionDistance(this->ambientOcclusionDistance);
	raytracer->setAmbientOcclusionSamples(this->ambientOcclusionSamples);
	raytracer->setAmbientOcclusionFactor(this->ambientOcclusionFactor);
	raytracer->setAmbientOcclusion(this->ambientOcclusion);
	raytracer->setThreads(this->threads);
	raytracer->setSamples(this->samples);
	raytracer->setShading(this->shading);
	raytracer->setCameraPosition(this->cameraPosition);
	raytracer->setCameraRotation(this->cameraRotation);
	raytracer->setCameraFov(this->cameraFov);
	for (size_t i = 0; i < this->objects.size(); ++i)
		raytracer->addObject(this->objects[i]);
	for (size_t i = 0; i < this->lights.size(); ++i)
		raytracer->addLight(this->lights[i]);
	if (this->denoising)
		raytracer->addFilter(new Denoising(this->denoisingRadius, this->denoisingThreshold));
	if (this->gamma)
		raytracer->addFilter(new Gamma(this->gammaValue));
	if (this->contrast)
		raytracer->addFilter(new Contrast(this->contrastValue));
	if (this->brightness)
		raytracer->addFilter(new Brightness(this->brightnessValue));
	if (this->saturation)
		raytracer->addFilter(new Saturation(this->saturationValue));
	if (this->depthOfField)
		raytracer->addFilter(new DepthOfField(this->depthOfFieldFocal, 1 / this->depthOfFieldAperture));
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
	if (this->glow)
		raytracer->addFilter(new Glow(this->glowRadius, this->glowThreshold, this->glowIntensity));
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
			continue;
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
			continue;
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
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Scale"))
		{
			Vec3 scale;
			parseVec3(child, &scale);
			sphere->setScale(scale);
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
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Scale"))
		{
			Vec3 scale;
			parseVec3(child, &scale);
			cylinder->setScale(scale);
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
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Scale"))
		{
			Vec3 scale;
			parseVec3(child, &scale);
			cone->setScale(scale);
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
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Scale"))
		{
			Vec3 scale;
			parseVec3(child, &scale);
			plane->setScale(scale);
			continue;
		}
	}
	this->objects.push_back(plane);
}

void Parser::parseTriangle(xmlNode *node)
{
	Triangle *triangle = new Triangle();
	std::string material;
	for (xmlAttr *attr = node->properties; attr; attr = attr->next)
	{
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("material"))
		{
			parseAttrString(attr, &material);
			continue;
		}
	}
	triangle->setMaterial(getMaterial(material));
	for (xmlNode *child = node->children; child; child = child->next)
	{
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Point1"))
		{
			Vec3 point;
			parseVec3(child, &point);
			triangle->setPoint1(point);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Point2"))
		{
			Vec3 point;
			parseVec3(child, &point);
			triangle->setPoint2(point);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Point3"))
		{
			Vec3 point;
			parseVec3(child, &point);
			triangle->setPoint3(point);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Position"))
		{
			Vec3 position;
			parseVec3(child, &position);
			triangle->setPosition(position);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Rotation"))
		{
			Vec3 rotation;
			parseVec3(child, &rotation);
			triangle->setRotation(rotation);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Scale"))
		{
			Vec3 scale;
			parseVec3(child, &scale);
			triangle->setScale(scale);
			continue;
		}
	}
	this->objects.push_back(triangle);
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
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("DiffuseTexture"))
		{
			parseTexture(child, &material->diffuseTexture);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("DiffuseColor"))
		{
			parseVec3(child, &material->diffuseColor);
			material->diffuseColor = clamp(material->diffuseColor, 0.f, 1.f);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("SpecularTexture"))
		{
			parseTexture(child, &material->specularTexture);
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
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("EmissiveTexture"))
		{
			parseTexture(child, &material->emissiveTexture);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("EmissiveColor"))
		{
			parseVec3(child, &material->emissiveColor);
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
			parseAttrUInt(attr, &this->blurRadius);
			continue;
		}
	}
	(void)node;
}

void Parser::parseFisheye(xmlNode *node)
{
	this->fisheye = true;
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

void Parser::parseGlow(xmlNode *node)
{
	this->glow = true;
	for (xmlAttr *attr = node->properties; attr; attr = attr->next)
	{
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("radius"))
		{
			parseAttrUInt(attr, &this->glowRadius);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("threshold"))
		{
			parseAttrFloat(attr, &this->glowThreshold);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("intensity"))
		{
			parseAttrFloat(attr, &this->glowIntensity);
			continue;
		}
	}
	(void)node;
}

void Parser::parseGamma(xmlNode *node)
{
	this->gamma = true;
	for (xmlAttr *attr = node->properties; attr; attr = attr->next)
	{
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("value"))
		{
			parseAttrFloat(attr, &this->gammaValue);
			continue;
		}
	}
	(void)node;
}

void Parser::parseBrightness(xmlNode *node)
{
	this->brightness = true;
	for (xmlAttr *attr = node->properties; attr; attr = attr->next)
	{
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("value"))
		{
			parseAttrFloat(attr, &this->brightnessValue);
			continue;
		}
	}
	(void)node;
}

void Parser::parseContrast(xmlNode *node)
{
	this->contrast = true;
	for (xmlAttr *attr = node->properties; attr; attr = attr->next)
	{
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("value"))
		{
			parseAttrFloat(attr, &this->contrastValue);
			continue;
		}
	}
	(void)node;
}

void Parser::parseSaturation(xmlNode *node)
{
	this->saturation = true;
	for (xmlAttr *attr = node->properties; attr; attr = attr->next)
	{
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("value"))
		{
			parseAttrFloat(attr, &this->saturationValue);
			continue;
		}
	}
	(void)node;
}

void Parser::parseDepthOfField(xmlNode *node)
{
	this->depthOfField = true;
	for (xmlAttr *attr = node->properties; attr; attr = attr->next)
	{
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("focal"))
		{
			parseAttrFloat(attr, &this->depthOfFieldFocal);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("aperture"))
		{
			parseAttrFloat(attr, &this->depthOfFieldAperture);
			continue;
		}
	}
	(void)node;
}

void Parser::parseDenoising(xmlNode *node)
{
	this->denoising = true;
	for (xmlAttr *attr = node->properties; attr; attr = attr->next)
	{
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("radius"))
		{
			parseAttrUInt(attr, &this->denoisingRadius);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("threshold"))
		{
			parseAttrFloat(attr, &this->denoisingThreshold);
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
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Glow"))
		{
			parseGlow(child);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Gamma"))
		{
			parseGamma(child);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Brightness"))
		{
			parseBrightness(child);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Contrast"))
		{
			parseContrast(child);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Saturation"))
		{
			parseSaturation(child);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("DepthOfField"))
		{
			parseDepthOfField(child);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Denoising"))
		{
			parseDenoising(child);
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
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("globalIlluminationSamples"))
		{
			parseAttrUInt(attr, &this->globalIlluminationSamples);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("globalIlluminationDistamce"))
		{
			parseAttrFloat(attr, &this->globalIlluminationDistance);
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
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("ambientOcclusionSamples"))
		{
			parseAttrUInt(attr, &this->ambientOcclusionSamples);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("ambientOcclusionDistance"))
		{
			parseAttrFloat(attr, &this->ambientOcclusionDistance);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("ambientOcclusionFactor"))
		{
			parseAttrFloat(attr, &this->ambientOcclusionFactor);
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
	delete (*texture);
	*texture = new Image();
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
			(*texture)->setData(width, height, data);
			return;
		}
		if (!std::string(reinterpret_cast<const char*>(attr->name)).compare("filtering"))
		{
			std::string filtering;
			parseAttrString(attr, &filtering);
			if (!filtering.compare("nearest"))
				(*texture)->setFiltering(IMAGE_FILTERING_NEAREST);
			else if (!filtering.compare("linear"))
				(*texture)->setFiltering(IMAGE_FILTERING_LINEAR);
			else if (!filtering.compare("cubic"))
				(*texture)->setFiltering(IMAGE_FILTERING_CUBIC);
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
