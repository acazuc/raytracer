#ifndef PARSER_H
# define PARSER_H

# include "Vec/Vec4.h"
# include "Vec/Vec3.h"
# include "Vec/Vec2.h"
# include <libxml/tree.h>
# include <unordered_map>
# include <string>
# include <vector>

class Raytracer;
class Material;
class Object;
class Light;
class Image;

class Parser
{

	private:
		std::unordered_map<std::string, Material*> materials;
		std::vector<Object*> objects;
		std::vector<Light*> lights;
		std::string filename;
		Material *defaultMaterial;
		Vec3 cameraPosition;
		Vec3 cameraRotation;
		float cameraFov;
		Vec3 ambient;
		uint8_t threads;
		uint8_t samples;
		bool shading;
		size_t width;
		size_t height;
		uint8_t celShadingCels;
		float fisheyeAperture;
		size_t blurRadius;
		bool globalIllumination;
		bool ambientOcclusion;
		bool celShading;
		bool greyShade;
		bool negative;
		bool sobel;
		bool sepia;
		bool fxaa;
		bool blur;
		bool fisheye;
		bool parseAttrUInt(xmlAttr *attr, size_t *value);
		bool parseAttrInt(xmlAttr *attr, ssize_t *value);
		bool parseAttrFloat(xmlAttr *attr, float *value);
		bool parseAttrBool(xmlAttr *attr, bool *value);
		bool parseAttrString(xmlAttr *attr, std::string *value);
		void parseVec4(xmlNode *node, Vec4 *vec);
		void parseVec3(xmlNode *node, Vec3 *vec);
		void parseVec1(xmlNode *node, float *vec);
		void parseTexture(xmlNode *node, Image **texture);
		void parseDirectionalLight(xmlNode *node);
		void parsePunctualLight(xmlNode *node);
		void parseLights(xmlNode *node);
		void parseSphere(xmlNode *node);
		void parseCone(xmlNode *node);
		void parseCylinder(xmlNode *node);
		void parsePlane(xmlNode *node);
		void parseTriangle(xmlNode *node);
		void parseObjects(xmlNode *node);
		void parseMaterial(xmlNode *node);
		void parseMaterials(xmlNode *node);
		void parseSobel(xmlNode *node);
		void parseGreyShade(xmlNode *node);
		void parseCelShading(xmlNode *node);
		void parseFxaa(xmlNode *node);
		void parseNegative(xmlNode *node);
		void parseSepia(xmlNode *node);
		void parseBlur(xmlNode *node);
		void parseFisheye(xmlNode *node);
		void parseFilters(xmlNode *node);
		void parseCamera(xmlNode *node);
		void parseEnvironment(xmlNode *node);
		void parseScene(xmlNode *node);
		Material *getMaterial(std::string name);

	public:
		Parser(std::string filename);
		bool parse();
		Raytracer *createRaytracer();
		inline std::unordered_map<std::string, Material*> &getMaterials() {return materials;};

};

#endif
