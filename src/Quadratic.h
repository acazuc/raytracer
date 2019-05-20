#ifndef QUADRATIC_H
# define QUADRATIC_H

# include <cstdint>

class Quadratic
{

	public:
		uint8_t solutions;
		float minT;
		float t1;
		float t2;
		float a;
		float b;
		float c;
		float d;
		bool solve();
		float getMinPosT();

};

#endif
