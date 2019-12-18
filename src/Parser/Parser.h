#ifndef PARSER_H
# define PARSER_H

# include "Math/Vec4.h"
# include "Math/Vec3.h"
# include "Math/Vec2.h"
# include <unordered_map>
# include <libxml/tree.h>
# include <string>

class Texture;
class Parser;

using ParserAttributeCallback = void(Parser::*)(xmlAttr *attr);
using ParserNodeCallback = void(Parser::*)(xmlNode *node);

class Parser
{

	private:
		std::unordered_map<std::string, ParserAttributeCallback> attributesCallbacks;
		std::unordered_map<std::string, ParserNodeCallback> nodesCallbacks;

	protected:
		void registerAttributeCallback(std::string name, ParserAttributeCallback callback);
		void registerNodeCallback(std::string name, ParserNodeCallback callback);
		bool parseAttrUInt(xmlAttr *attr, size_t *value);
		bool parseAttrInt(xmlAttr *attr, ssize_t *value);
		bool parseAttrFloat(xmlAttr *attr, float *value);
		bool parseAttrBool(xmlAttr *attr, bool *value);
		bool parseAttrString(xmlAttr *attr, std::string *value);
		void parseVec4(xmlNode *node, Vec4 *vec);
		void parseVec3(xmlNode *node, Vec3 *vec);
		void parseVec2(xmlNode *node, Vec2 *vec);
		void parseVec1(xmlNode *node, float *vec);
		void parseTexture(xmlNode *node, Texture **texture);

	public:
		Parser();
		virtual ~Parser();
		void parseAttributes(xmlNode *node);
		bool parseAttribute(xmlAttr *attr);
		void parseChilds(xmlNode *node);
		bool parseChild(xmlNode *child);
		void parse(xmlNode *node);

};

#endif
