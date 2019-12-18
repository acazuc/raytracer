#include "ObjParser.h"
#include "Objects/Triangle.h"
#include "../FileParser.h"
#include "Raytracer.h"
#include "Debug.h"
#include <fstream>

ObjParser::ObjParser(FileParser &file)
: ModelParser(file)
{
	registerAttributeCallback("file", ParserAttributeCallback(&ObjParser::parseFile));
	this->positions.push_back(Vec3(0));
	this->normals.push_back(Vec3(0));
	this->uvs.push_back(Vec2(0));
}

void ObjParser::parseFile(xmlAttr *attr)
{
	if (!attr->children)
		return;
	std::ifstream ifs(std::string(reinterpret_cast<const char*>(attr->children->content)), std::ios_base::in);
	if (!ifs)
		return;
	std::string line;
	while (std::getline(ifs, line))
	{
		if (!line.compare(0, 2, "v "))
		{
			parsePosition(line);
			continue;
		}
		if (!line.compare(0, 3, "vn "))
		{
			parseNormal(line);
			continue;
		}
		if (!line.compare(0, 3, "vt "))
		{
			parseUV(line);
			continue;
		}
		if (!line.compare(0, 2, "f "))
		{
			parseFace(line);
			continue;
		}
	}
}

void ObjParser::parsePosition(std::string &line)
{
	try
	{
		Vec3 p;
		size_t pos = line.find(' ', 2);
		if (pos == std::string::npos)
			return;
		p.x = std::stof(line.substr(2, pos - 2));
		size_t pos2 = line.find(' ', pos + 1);
		if (pos2 == std::string::npos)
			return;
		p.y = std::stof(line.substr(pos + 1, (pos2 - pos + 1)));
		p.z = std::stof(line.substr(pos2 + 1, line.length() - (pos2 + 1)));
		this->positions.push_back(p);
	}
	catch (std::exception &e)
	{
		return;
	}
}

void ObjParser::parseNormal(std::string &line)
{
	try
	{
		Vec3 p;
		size_t pos = line.find(' ', 2);
		if (pos == std::string::npos)
			return;
		p.x = std::stof(line.substr(2, pos - 2));
		size_t pos2 = line.find(' ', pos + 1);
		if (pos2 == std::string::npos)
			return;
		p.y = std::stof(line.substr(pos + 1, pos2 - (pos + 1)));
		pos = line.find(' ', pos2 + 1);
		if (pos == std::string::npos)
			return;
		p.z = std::stof(line.substr(pos2 + 1, pos - (pos2 + 1)));
		this->normals.push_back(p);
	}
	catch (std::exception &e)
	{
		return;
	}
}

void ObjParser::parseUV(std::string &line)
{
	try
	{
		Vec2 p;
		size_t pos = line.find(' ', 2);
		if (pos == std::string::npos)
			return;
		p.x = std::stof(line.substr(2, pos - 2));
		size_t pos2 = line.find(' ', pos + 1);
		if (pos2 == std::string::npos)
			return;
		p.y = std::stof(line.substr(pos + 1, pos2 - (pos + 1)));
		this->uvs.push_back(p);
	}
	catch (std::exception &e)
	{
		return;
	}
}

bool ObjParser::parseFacePoint(std::string line, TVec3<size_t> &p)
{
	try
	{
		size_t pos2 = line.find('/', 0);
		if (pos2 == std::string::npos)
		{
			p.x = std::stoull(line);
			p.y = 0;
			p.z = 0;
			return true;
		}
		p.x = std::stoull(line.substr(0, pos2));
		size_t pos = line.find('/', pos2 + 1);
		if (pos == std::string::npos)
		{
			p.y = 0;
			p.z = 0;
			return true;
		}
		if (pos == pos2 + 1)
			p.y = 0;
		else
			p.y = std::stoull(line.substr(pos2 + 1, pos - (pos2 + 1)));
		if (pos + 1 == line.length())
			p.z = 0;
		else
			p.z = std::stoull(line.substr(pos + 1, line.length() - pos));
	}
	catch (std::exception &e)
	{
		return false;
	}
	return true;
}

void ObjParser::parseFace(std::string &line)
{
	try
	{
		TVec3<size_t> p;
		size_t pos = line.find(' ', 2);
		if (pos == std::string::npos)
			return;
		if (!parseFacePoint(line.substr(2, pos), p))
			return;
		this->indices.push_back(p);
		size_t pos2 = line.find(' ', pos + 1);
		if (pos2 == std::string::npos)
			return;
		if (!parseFacePoint(line.substr(pos + 1, pos2 - (pos + 1)), p))
			return;
		this->indices.push_back(p);
		if (!parseFacePoint(line.substr(pos2 + 1, line.length() - (pos2 + 1)), p))
			return;
		this->indices.push_back(p);
	}
	catch (std::exception &e)
	{
		return;
	}
}

void ObjParser::addToRaytracer(Raytracer *raytracer)
{
	for (size_t i = 0; i < this->indices.size(); i++)
	{
		Triangle *triangle = new Triangle();
		if (!this->indices[i].x || this->indices[i].x > this->positions.size())
		{
			delete (triangle);
			i += 2;
			continue;
		}
		triangle->setPoint1(this->positions[this->indices[i].x] + this->position);
		if (this->indices[i].y && this->indices[i].y < this->uvs.size())
			triangle->setUV1(this->uvs[this->indices[i].z]);
		if (this->indices[i].z && this->indices[i].z < this->normals.size())
			triangle->setNorm1(this->normals[this->indices[i].y]);
		i++;
		if (!this->indices[i].x || this->indices[i].x > this->positions.size())
		{
			delete (triangle);
			i++;
			continue;
		}
		triangle->setPoint2(this->positions[this->indices[i].x] + this->position);
		if (this->indices[i].y && this->indices[i].y < this->uvs.size())
			triangle->setUV2(this->uvs[this->indices[i].z]);
		if (this->indices[i].z && this->indices[i].z < this->normals.size())
			triangle->setNorm2(this->normals[this->indices[i].y]);
		i++;
		if (!this->indices[i].x || this->indices[i].x > this->positions.size())
		{
			delete (triangle);
			continue;
		}
		triangle->setPoint3(this->positions[this->indices[i].x] + this->position);
		if (this->indices[i].y && this->indices[i].y < this->uvs.size())
			triangle->setUV3(this->uvs[this->indices[i].z]);
		if (this->indices[i].z && this->indices[i].z < this->normals.size())
			triangle->setNorm3(this->normals[this->indices[i].y]);
		triangle->material = this->file.getDefaultMaterial();
		raytracer->addObject(triangle);
	}
}
