#ifndef IMAGE_H
# define IMAGE_H

# include "Vec/Vec4.h"
# include "Vec/Vec2.h"
# include <cstdint>
# include <vector>

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
		std::vector<Vec4> data;
		uint32_t height;
		uint32_t width;
		Image(uint32_t width, uint32_t height);
		Image();
		void setData(uint32_t width, uint32_t height, uint8_t *data);
		Vec4 getTexelAt(int32_t x, int32_t y);
		Vec4 getDataAt(Vec2 uv);

};

#endif
