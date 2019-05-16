#ifndef MATERIAL_H
# define MATERIAL_H

# include "Vec/Vec4.h"
# include "Vec/Vec3.h"

class Image;

class Material
{

	public:
		Image *diffuseTexture;
		Image *normalTexture;
		Vec3 specularColor;
		Vec3 diffuseColor;
		Vec3 ambientColor;
		float specularFactor;
		float reflection;
		float refraction;
		float opacity;
		Material();

};

#endif
