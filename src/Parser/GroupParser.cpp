#include "GroupParser.h"
#include "./Models/ObjParser.h"
#include "./MaterialParser.h"
#include "./ObjectsParser.h"
#include "./LightsParser.h"
#include "./FileParser.h"
#include "Raytracer.h"

GroupParser::GroupParser(FileParser &file, GroupParser *parent)
: parent(parent)
, file(file)
{
	registerNodeCallback("Position", ParserNodeCallback(&GroupParser::parsePosition));
	registerNodeCallback("Rotation", ParserNodeCallback(&GroupParser::parseRotation));
	registerNodeCallback("Scale", ParserNodeCallback(&GroupParser::parseScale));
	registerNodeCallback("Materials", ParserNodeCallback(&GroupParser::parseMaterials));
	registerNodeCallback("Objects", ParserNodeCallback(&GroupParser::parseObjects));
	registerNodeCallback("Lights", ParserNodeCallback(&GroupParser::parseLights));
	registerNodeCallback("Groups", ParserNodeCallback(&GroupParser::parseGroups));
}

void GroupParser::parsePosition(xmlNode *node)
{
	parseVec3(node, &this->position);
}

void GroupParser::parseRotation(xmlNode *node)
{
	parseVec3(node, &this->rotation);
}

void GroupParser::parseScale(xmlNode *node)
{
	parseVec3(node, &this->scale);
}

void GroupParser::parseMaterials(xmlNode *node)
{
	for (xmlNode *child = node->children; child; child = child->next)
	{
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Material"))
		{
			MaterialParser *material = new MaterialParser(this->file);
			material->parse(child);
			this->materials.push_back(material);
			continue;
		}
	}
}

void GroupParser::parseObjects(xmlNode *node)
{
	for (xmlNode *child = node->children; child; child = child->next)
	{
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Sphere"))
		{
			SphereParser *sphere = new SphereParser(this->file, *this);
			sphere->parse(child);
			this->objects.push_back(sphere);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Cone"))
		{
			ConeParser *cone = new ConeParser(this->file, *this);
			cone->parse(child);
			this->objects.push_back(cone);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Cylinder"))
		{
			CylinderParser *cylinder = new CylinderParser(this->file, *this);
			cylinder->parse(child);
			this->objects.push_back(cylinder);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Plane"))
		{
			PlaneParser *plane = new PlaneParser(this->file, *this);
			plane->parse(child);
			this->objects.push_back(plane);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Triangle"))
		{
			TriangleParser *triangle = new TriangleParser(this->file, *this);
			triangle->parse(child);
			this->objects.push_back(triangle);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("ObjModel"))
		{
			ObjParser *model = new ObjParser(this->file);
			model->parse(child);
			this->models.push_back(model);
			continue;
		}
	}
}

void GroupParser::parseLights(xmlNode *node)
{
	for (xmlNode *child = node->children; child; child = child->next)
	{
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("PunctualLight"))
		{
			PunctualLightParser *light = new PunctualLightParser(this->file, *this);
			light->parse(child);
			this->lights.push_back(light);
			continue;
		}
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("DirectionalLight"))
		{
			DirectionalLightParser *light = new DirectionalLightParser(this->file, *this);
			light->parse(child);
			this->lights.push_back(light);
			continue;
		}
	}
}

void GroupParser::parseGroups(xmlNode *node)
{
	for (xmlNode *child = node->children; child; child = child->next)
	{
		if (!std::string(reinterpret_cast<const char*>(child->name)).compare("Group"))
		{
			GroupParser *group = new GroupParser(this->file, this);
			group->parse(child);
			this->groups.push_back(group);
		}
	}
}

void GroupParser::addMaterials(Raytracer *raytracer)
{
	for (GroupParser *group : this->groups)
		group->addMaterials(raytracer);
	for (MaterialParser *material : this->materials)
	{
		Material *mt = material->toMaterial();
		raytracer->addMaterial(mt);
		this->file.registerMaterial(material->getName(), mt);
	}
}

void GroupParser::addObjects(Raytracer *raytracer)
{
	for (GroupParser *group : this->groups)
		group->addObjects(raytracer);
	for (ObjectParser *object : this->objects)
		object->addToRaytracer(raytracer);
	for (ModelParser *model : this->models)
		model->addToRaytracer(raytracer);
}

void GroupParser::addLights(Raytracer *raytracer)
{
	for (GroupParser *group : this->groups)
		group->addObjects(raytracer);
	for (LightParser *light : this->lights)
		light->addToRaytracer(raytracer);
}
