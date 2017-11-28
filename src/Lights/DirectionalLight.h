#ifndef DIRECTIONAL_LIGHT_H
# define DIRECTIONAL_LIGHT_H

# include "Light.h"

class DirectionalLight : public Light
{

	public:
		Vec3 dir;
		Vec3 getDirectionFrom(Vec3 &pos);
};

#endif
