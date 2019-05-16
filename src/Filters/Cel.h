#ifndef CEL_H
# define CEL_H

# include "./Filter.h"

class Cel : public Filter
{

	private:
		uint8_t cels;

	public:
		Cel(uint8_t cels);
		void operator()(Vec4 *dst, Vec4 *src, float *zBuffer, size_t width, size_t height);

};

#endif
