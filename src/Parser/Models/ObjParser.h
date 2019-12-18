#ifndef OBJ_PARSER_H
# define OBJ_PARSER_H

# include "./ModelParser.h"
# include <vector>

class ObjParser : public ModelParser
{

	private:
		std::vector<Vec3> positions;
		std::vector<Vec3> normals;
		std::vector<Vec2> uvs;
		std::vector<TVec3<size_t>> indices;
		void parseFile(xmlAttr *attr);
		void parsePosition(std::string &line);
		void parseNormal(std::string &line);
		void parseUV(std::string &line);
		bool parseFacePoint(std::string line, TVec3<size_t> &p);
		void parseFace(std::string &line);

	public:
		ObjParser(FileParser &file);
		void addToRaytracer(Raytracer *raytracer);

};

#endif
