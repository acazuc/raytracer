#ifndef LIGHTS_PARSER_H
# define LIGHTS_PARSER_H

# include "./Parser.h"

class GroupParser;
class FileParser;
class Light;

class LightParser : public Parser
{

	protected:
		GroupParser &parent;
		FileParser &file;
		Vec3 color;
		float intensity;
		void parseColor(xmlNode *node);
		void parseIntensity(xmlAttr *attr);

	public:
		LightParser(FileParser &file, GroupParser &parent);
		virtual Light *toLight() = 0;

};

class PunctualLightParser : public LightParser
{

	private:
		Vec3 position;
		void parsePosition(xmlNode *node);

	public:
		PunctualLightParser(FileParser &file, GroupParser &parent);
		Light *toLight();

};

class DirectionalLightParser : public LightParser
{

	private:
		Vec3 direction;
		void parseDirection(xmlNode *node);

	public:
		DirectionalLightParser(FileParser &file, GroupParser &parent);
		Light *toLight();

};

#endif
