#pragma once

#include "../Def.h"
#include "../util.h"

#include <vector>

namespace Ansel
{
	enum FrameBufferType {
		COLOR,
		STENCIL,
		DEPTH,
		FONT,
		NONE
	} typedef FrameBufferType;

	class Texture
	{
		unsigned int	ID;
		std::string		filename;
		unsigned char*	localBuffer;

		int width, height, BBP;

	public:
		ANSEL_API Texture();
		ANSEL_API Texture(const int w, const int h, unsigned char* buffer = nullptr, FrameBufferType type = NONE);
		ANSEL_API Texture(const std::string &path);
		ANSEL_API ~Texture();

		void ANSEL_API bind(unsigned int slot = 0) const;
		void ANSEL_API unBind();

		/**
		  * Get texture coordinates for a rectangle on this texture.
		  * @param[in] column_count Amount of columns in the image
		  * @param[in] row_count    Amount of rows in the image
		  * @param[in] x            x index location of the rectangle
		  * @param[in] y            y index location of the rectangle
		  */
		std::vector<vec2f> ANSEL_API getTexRect(const int column_count, const int row_count, const int x, const int y);

		inline int getWidth()  const { return width;  }
		inline int getHeight() const { return height; }

		inline unsigned int getID() const { return ID; }
	};
}