#ifndef PONCTUAL_LIGHT_H
# define PONCTUAL_LIGHT_H

# include "Light.h"

class PonctualLight : public Light
{

	public:
		Vec3 pos;
		Vec3 getDirectionFrom(Vec3 &pos);

};

#endif
