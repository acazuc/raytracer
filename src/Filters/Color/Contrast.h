#ifndef CONTRAST_H
# define CONTRAST_H

# include "../Filter.h"

class Contrast : public Filter
{

	private:
		float contrast;

	public:
		Contrast(float contrast);
		Vec4 operator()(Vec4 *colorBuffer, float *zBuffer, size_t x, size_t y, size_t width, size_t height);

};

#endif
