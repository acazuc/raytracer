#ifndef CEL_H
# define CEL_H

# include "./Filter.h"

class Cel : public Filter
{

	private:
		uint8_t cels;

	public:
		Cel(uint8_t cels);
		Vec4 operator()(Vec4 *colorBuffer, float *zBuffer, size_t x, size_t y, size_t width, size_t height);

};

#endif
