#ifndef CEL_H
# define CEL_H

# include "Vec/Vec4.h"
# include <cstdint>

class Cel
{

	public:
		static Vec4 *cel(Vec4 *img, uint8_t cels, uint64_t width, uint64_t height);

};

#endif
