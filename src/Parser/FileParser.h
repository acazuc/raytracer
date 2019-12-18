#ifndef FILE_PARSER_H
# define FILE_PARSER_H

# include "./FiltersParser.h"
# include "./Parser.h"
# include <libxml/tree.h>
# include <unordered_map>
# include <string>

class MaterialParser;
class ModelParser;
class GroupParser;
class Raytracer;
class Material;
class Object;
class Light;
class Image;

class FileParser : public Parser
{

	private:
		std::unordered_map<std::string, Material*> materials;
		Material *defaultMaterial;
		FiltersParser filters;
		std::string filename;
		GroupParser *group;
		Vec3 cameraPosition;
		Vec3 cameraRotation;
		size_t maxReflection;
		float cameraFov;
		uint8_t threads;
		uint8_t samples;
		bool shading;
		size_t width;
		size_t height;
		size_t globalIlluminationSamples;
		float globalIlluminationFactor;
		bool globalIllumination;
		void parseFilters(xmlNode *node);
		void parseCamera(xmlNode *node);
		void parseScene(xmlNode *node);

	public:
		FileParser(std::string filename);
		void registerMaterial(std::string name, Material *material);
		Material *getMaterial(std::string name);
		bool parse();
		Raytracer *createRaytracer();
		inline Material *getDefaultMaterial() {return this->defaultMaterial;};

};

#endif
