#ifndef DIRECTIONAL_LIGHT_H
# define DIRECTIONAL_LIGHT_H

# include "Light.h"

class DirectionalLight : public Light
{

	private:
		Vec3 direction;

	public:
		Vec3 getDirectionFrom(Vec3 &pos);
		void setDirection(Vec3 direction);
};

#endif
