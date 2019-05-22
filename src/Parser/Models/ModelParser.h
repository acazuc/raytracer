#ifndef MODEL_PARSER_H
# define MODEL_PARSER_H

# include "Vec/Vec3.h"
# include <string>

class ModelParser
{

	protected:
		std::string filename;
		Vec3 position;
		Vec3 rotation;
		Vec3 scale;

	public:
		ModelParser(std::string filename);
		virtual ~ModelParser() {};
		virtual void parse() = 0;
		void setPosition(Vec3 position);
		void setRotation(Vec3 rotation);
		void setScale(Vec3 scale);

};

#endif
