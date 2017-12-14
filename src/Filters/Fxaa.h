#ifndef FXAA_H
# define FXAA_H

# include "Vec/Vec4.h"
# include <cstdint>

class Fxaa
{

	public:
		static Vec4 *fxaa(Vec4 *img, uint64_t width, uint64_t height);

};

#endif
