#ifndef MODEL_PARSER_H
# define MODEL_PARSER_H

# include "../Parser.h"
# include "Math/Vec3.h"

class FileParser;
class Raytracer;

class ModelParser : public Parser
{

	protected:
		FileParser &file;
		Vec3 position;
		Vec3 rotation;
		Vec3 scale;
		void parsePosition(xmlNode *node);
		void parseRotation(xmlNode *node);
		void parseScale(xmlNode *node);

	public:
		ModelParser(FileParser &file);
		virtual void addToRaytracer(Raytracer *raytracer) = 0;

};

#endif
