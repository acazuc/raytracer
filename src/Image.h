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
		size_t height;
		size_t width;
		Image(size_t width, size_t height);
		Image();
		void setData(size_t width, size_t height, const uint8_t *data);
		Vec4 getTexelAt(ssize_t x, ssize_t y);
		Vec4 getDataAt(Vec2 uv);
		void setFiltering(enum ImageFilteringMode filtering) {this->filtering = filtering;};

};

#endif
