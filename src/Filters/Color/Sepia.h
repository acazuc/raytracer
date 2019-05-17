#ifndef SEPIA_H
# define SEPIA_H

# include "../Filter.h"

class Sepia : public Filter
{

	public:
		Vec4 operator()(Vec4 *colorBuffer, float *zBuffer, size_t x, size_t y, size_t width, size_t height);

};

#endif
