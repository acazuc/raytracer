#ifndef MATERIAL_PARSER_H
# define MATERIAL_PARSER_H

# include "./Parser.h"

class FileParser;
class Material;
class Texture;

class MaterialParser : public Parser
{

	private:
		FileParser &file;
		std::string name;
		Texture *emissiveTexture;
		Texture *specularTexture;
		Texture *diffuseTexture;
		Texture *normalTexture;
		Vec3 emissiveColor;
		Vec3 specularColor;
		Vec3 diffuseColor;
		float specularFactor;
		float reflection;
		float refraction;
		float opacity;
		void parseName(xmlAttr *attr);
		void parseReflection(xmlAttr *attr);
		void parseRefraction(xmlAttr *attr);
		void parseTransparency(xmlAttr *attr);
		void parseOpacity(xmlAttr *attr);
		void parseEmissiveTexture(xmlNode *node);
		void parseSpecularTexture(xmlNode *node);
		void parseDiffuseTexture(xmlNode *node);
		void parseNormalTexture(xmlNode *node);
		void parseEmissiveColor(xmlNode *node);
		void parseSpecularColor(xmlNode *node);
		void parseDiffuseColor(xmlNode *node);
		void parseSpecularFactor(xmlNode *node);

	public:
		MaterialParser(FileParser &file);
		Material *toMaterial();
		inline std::string &getName() {return this->name;};

};

#endif
