#ifndef SEPIA_H
# define SEPIA_H

# include "../Filter.h"

class Sepia : public Filter
{

	public:
		void operator()(Vec4 *dst, Vec4 *src, float *zBuffer, size_t width, size_t height);

};

#endif
