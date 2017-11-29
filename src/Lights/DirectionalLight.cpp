#include "DirectionalLight.h"

Vec3 DirectionalLight::getDirectionFrom(Vec3 &pos)
{
	(void)pos;
	Vec3 vec = -dir;
	return (vec);
}
