#ifndef IMAGE_H
# define IMAGE_H

# include "Vec/Vec4.h"
# include "Vec/Vec2.h"
# include <cstdint>

enum ImageWrapMode
{
	IMAGE_WRAP_REPEAT,
	IMAGE_WRAP_MIRRORED_REPEAT,
	IMAGE_WRAP_CLAMP,
};

enum ImageFilteringMode
{
	IMAGE_FILTERING_NEAREST,
	IMAGE_FILTERING_LINEAR,
	IMAGE_FILTERING_CUBIC
};

class Image
{

	public:
		enum ImageFilteringMode filtering;
		enum ImageWrapMode wrap;
		uint32_t height;
		uint32_t width;
		Vec4 *data;
		Image(uint32_t width, uint32_t height);
		Image();
		void setData(uint64_t width, uint64_t height, uint8_t *data);
		Vec4 getTexelAt(uint32_t x, uint32_t y);
		Vec4 getDataAt(Vec2 uv);

};

#endif
