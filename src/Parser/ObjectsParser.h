#ifndef OBJECTS_PARSER_H
# define OBJECTS_PARSER_H

# include "./Parser.h"
# include <libxml/tree.h>
# include <string>

class GroupParser;
class FileParser;
class Raytracer;

class ObjectParser : public Parser
{

	protected:
		GroupParser &parent;
		FileParser &file;
		std::string material;
		Vec3 position;
		Vec3 rotation;
		Vec3 scale;
		void parseMaterial(xmlAttr *attr);
		void parsePosition(xmlNode *node);
		void parseRotation(xmlNode *node);
		void parseScale(xmlNode *node);

	public:
		ObjectParser(FileParser &file, GroupParser &parent);
		virtual ~ObjectParser();
		virtual void addToRaytracer(Raytracer *raytracer) = 0;

};

class SphereParser : public ObjectParser
{

	private:
		float size;
		void parseSize(xmlAttr *attr);

	public:
		SphereParser(FileParser &file, GroupParser &parent);
		void addToRaytracer(Raytracer *raytracer);

};

class CylinderParser : public ObjectParser
{

	private:
		float size;
		void parseSize(xmlAttr *attr);

	public:
		CylinderParser(FileParser &file, GroupParser &parent);
		void addToRaytracer(Raytracer *raytracer);

};

class ConeParser : public ObjectParser
{

	private:
		float size;
		void parseSize(xmlAttr *attr);

	public:
		ConeParser(FileParser &file, GroupParser &parent);
		void addToRaytracer(Raytracer *raytracer);

};

class PlaneParser : public ObjectParser
{

	private:
		Vec3 normal;
		void parseNormal(xmlNode *node);

	public:
		PlaneParser(FileParser &file, GroupParser &parent);
		void addToRaytracer(Raytracer *raytracer);

};

class TriangleParser : public ObjectParser
{

	private:
		Vec3 point1;
		Vec3 point2;
		Vec3 point3;
		Vec3 norm1;
		Vec3 norm2;
		Vec3 norm3;
		Vec2 UV1;
		Vec2 UV2;
		Vec2 UV3;
		void parsePoint1(xmlNode *node);
		void parsePoint2(xmlNode *node);
		void parsePoint3(xmlNode *node);
		void parseNorm1(xmlNode *node);
		void parseNorm2(xmlNode *node);
		void parseNorm3(xmlNode *node);
		void parseUV1(xmlNode *node);
		void parseUV2(xmlNode *node);
		void parseUV3(xmlNode *node);

	public:
		TriangleParser(FileParser &file, GroupParser &parent);
		void addToRaytracer(Raytracer *raytracer);

};

#endif
