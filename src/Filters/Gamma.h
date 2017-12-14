#ifndef GAMMA_H
# define GAMMA_H

# include "Vec/Vec4.h"
# include <cstdint>

class Gamma
{

	public:
		static Vec4 *gamma(Vec4 *img, uint64_t width, uint64_t height, float gamma);

};

#endif
