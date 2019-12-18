#ifndef TEXTURE_H
# define TEXTURE_H

# include "Math/Vec4.h"
# include "Math/Vec2.h"
# include <cstdint>
# include <vector>

enum TextureWrapMode
{
	TEXTURE_WRAP_REPEAT,
	TEXTURE_WRAP_MIRRORED_REPEAT,
	TEXTURE_WRAP_CLAMP,
};

enum TextureFilteringMode
{
	TEXTURE_FILTERING_NEAREST,
	TEXTURE_FILTERING_LINEAR,
	TEXTURE_FILTERING_CUBIC
};

class Texture
{

	public:
		enum TextureFilteringMode filtering;
		enum TextureWrapMode wrapS;
		enum TextureWrapMode wrapT;
		std::vector<Vec4> data;
		size_t height;
		size_t width;
		Texture(size_t width, size_t height);
		Texture();
		void setData(size_t width, size_t height, const uint8_t *data);
		Vec4 getTexelAt(ssize_t x, ssize_t y);
		Vec4 getDataAt(Vec2 uv);
		void setFiltering(enum TextureFilteringMode filtering);
		void setWrapS(enum TextureWrapMode wrap);
		void setWrapT(enum TextureWrapMode wrap);
		void setWrap(enum TextureWrapMode wrap);

};

#endif
