#ifndef LIGHT_H
# define LIGHT_H

# include "Vec3.h"

class Light
{

	public:
		Vec3 color;
		float intensity;
		virtual Vec3 getDirectionFrom(Vec3 &pos) {(void)pos;return (Vec3(0));};

};

#endif
