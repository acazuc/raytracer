#ifndef BRIGHTNESS_H
# define BRIGHTNESS_H

# include "../Filter.h"

class Brightness : public Filter
{

	private:
		float brightness;

	public:
		Brightness(float brightness);
		Vec4 operator()(Vec4 *colorBuffer, float *zBuffer, size_t x, size_t y, size_t width, size_t height);

};

#endif
