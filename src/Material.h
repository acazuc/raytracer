#ifndef MATERIAL_H
# define MATERIAL_H

# include "Math/Vec4.h"
# include "Math/Vec3.h"

class Texture;

class Material
{

	public:
		Texture *specularTexture;
		Texture *emissiveTexture;
		Texture *diffuseTexture;
		Texture *normalTexture;
		Vec3 emissiveColor;
		Vec3 specularColor;
		Vec3 diffuseColor;
		float specularFactor;
		float reflection;
		float refraction;
		float opacity;
		Material();

};

#endif
