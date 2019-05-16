#ifndef NEGATIVE_H
# define NEGATIVE_H

# include "../Filter.h"

class Negative : public Filter
{

	public:
		void operator()(Vec4 *dst, Vec4 *src, float *zBuffer, size_t width, size_t height);

};

#endif
