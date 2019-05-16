#ifndef FISHEYE_H
# define FISHEYE_H

# include "./Filter.h"

class Fisheye : public Filter
{

	private:
		float aperture;

	public:
		Fisheye(float aperture);
		void operator()(Vec4 *dst, Vec4 *src, float *zBuffer, size_t width, size_t height);

};

#endif
