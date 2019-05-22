#ifndef LIGHT_H
# define LIGHT_H

# include "Vec/Vec3.h"

class Light
{

	public:
		Vec3 color;
		virtual ~Light() {};
		virtual Vec3 getDirectionFrom(Vec3 &pos) = 0;
		void setColor(Vec3 color);

};

#endif
