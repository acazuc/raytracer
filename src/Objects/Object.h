#ifndef OBJECT_H
# define OBJECT_H

# include "Vec/Vec4.h"
# include "Vec/Vec3.h"
# include "Image.h"
# include "Ray.h"
# include <string>

class Object
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
		Object();
		virtual inline Vec3 *collide(Ray &ray) {(void)ray;return (nullptr);};
		virtual inline Vec2 getUVAt(Ray &ray, Vec3 &pos) {(void)ray;(void)pos;return (Vec2(0, 0));};
		virtual inline Vec3 getNormAt(Ray &ray, Vec3 &pos) {(void)ray;(void)pos;return (Vec3(0, 0, 1));};

};

#endif
