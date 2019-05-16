#ifndef PUNCTUAL_LIGHT_H
# define PUNCTUAL_LIGHT_H

# include "Light.h"

class PunctualLight : public Light
{

	private:
		Vec3 position;

	public:
		Vec3 getDirectionFrom(Vec3 &pos);
		void setPosition(Vec3 position);

};

#endif
