#ifndef FISHEYE_H
# define FISHEYE_H

# include "./Filter.h"

class Fisheye : public Filter
{

	private:
		float aperture;

	public:
		Fisheye(float aperture);
		Vec4 operator()(Vec4 *colorBuffer, float *zBuffer, size_t x, size_t y, size_t width, size_t height);

};

#endif
