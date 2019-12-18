#ifndef GROUP_PARSER_H
# define GROUP_PARSER_H

# include "./Parser.h"
# include <vector>

class MaterialParser;
class ObjectParser;
class LightParser;
class ModelParser;
class FileParser;
class Raytracer;

class GroupParser : public Parser
{

	private:
		std::vector<MaterialParser*> materials;
		std::vector<ObjectParser*> objects;
		std::vector<LightParser*> lights;
		std::vector<GroupParser*> groups;
		std::vector<ModelParser*> models;
		GroupParser *parent;
		FileParser &file;
		Vec3 position;
		Vec3 rotation;
		Vec3 scale;
		void parsePosition(xmlNode *node);
		void parseRotation(xmlNode *node);
		void parseScale(xmlNode *node);
		void parseMaterials(xmlNode *node);
		void parseObjects(xmlNode *node);
		void parseLights(xmlNode *node);
		void parseGroups(xmlNode *node);

	public:
		GroupParser(FileParser &file, GroupParser *parent);
		void addMaterials(Raytracer *raytracer);
		void addObjects(Raytracer *raytracer);
		void addLights(Raytracer *raytracer);

};

#endif
