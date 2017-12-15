#ifndef OBJECT_H
# define OBJECT_H

# include "Mat/Mat3.h"
# include "Vec/Vec4.h"
# include "Vec/Vec3.h"
# include "Image.h"
# include "Ray.h"
# include <string>

class Object
{

	public:
		Image *Kd_map;
		Image *N_map;
		Mat3 unrotMat;
		Mat3 rotMat;
		Vec3 pos;
		Vec4 Ka;
		Vec4 Kd;
		Vec4 Ks;
		float Ns;
		float Ni;
		float Ir;
		Object();
		void setRot(Vec3 vec);
		virtual inline bool collide(Ray &ray, float &t) {(void)ray;(void)t;return (false);};
		virtual inline Vec2 getUVAt(Ray &ray, Vec3 &pos) {(void)ray;(void)pos;return (Vec2(0, 0));};
		virtual inline Vec3 getNormAt(Ray &ray, Vec3 &pos) {(void)ray;(void)pos;return (Vec3(0, 0, 1));};

};

#endif
