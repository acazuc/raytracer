#ifndef DEPTH_OF_FIELD_H
# define DEPTH_OF_FIELD_H

# include "./Filter.h"

class DepthOfField : public Filter
{

	private:
		float aperture;
		float focal;

	public:
		DepthOfField(float focal, float aperture);
		Vec4 operator()(Vec4 *colorBuffer, float *zBuffer, size_t x, size_t y, size_t width, size_t height);

};

#endif
