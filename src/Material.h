#ifndef MATERIAL_H
# define MATERIAL_H

# include "Vec/Vec4.h"
# include "Vec/Vec3.h"
# include "Image.h"

class Material
{

	public:
		Image *bumpTexture;
		Image *texture;
		Vec3 pos;
		Vec3 rot;
		Vec4 Ka;
		Vec4 Kd;
		Vec4 Ks;
		float Ns;
		float Ni;
		float Ir;

};

#endif
