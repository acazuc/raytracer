#ifndef OBJECTS_PARSER_H
# define OBJECTS_PARSER_H

# include "./Parser.h"
# include <libxml/tree.h>
# include <string>

class GroupParser;
class FileParser;
class Object;

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
		virtual Object *toObject() = 0;

};

class SphereParser : public ObjectParser
{

	private:
		float size;
		void parseSize(xmlAttr *attr);

	public:
		SphereParser(FileParser &file, GroupParser &parent);
		Object *toObject();

};

class CylinderParser : public ObjectParser
{

	private:
		float size;
		void parseSize(xmlAttr *attr);

	public:
		CylinderParser(FileParser &file, GroupParser &parent);
		Object *toObject();

};

class ConeParser : public ObjectParser
{

	private:
		float size;
		void parseSize(xmlAttr *attr);

	public:
		ConeParser(FileParser &file, GroupParser &parent);
		Object *toObject();

};

class PlaneParser : public ObjectParser
{

	private:
		Vec3 normal;
		void parseNormal(xmlNode *node);

	public:
		PlaneParser(FileParser &file, GroupParser &parent);
		Object *toObject();

};

class TriangleParser : public ObjectParser
{

	private:
		//TODO

	public:
		TriangleParser(FileParser &file, GroupParser &parent);
		Object *toObject();

};

#endif
