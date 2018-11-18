#pragma once

#include "Def.h"

namespace Ansel
{
	class Texture
	{
		unsigned int	ID;
		std::string		filename;
		unsigned char*	localBuffer;

		int width, height, BBP;

	public:
		ANSEL_API Texture();
		ANSEL_API Texture(const std::string &path);
		ANSEL_API ~Texture();

		void ANSEL_API bind(unsigned int slot = 0) const;
		void ANSEL_API unBind();

		inline int getWidth()  const { return width;  }
		inline int getHeight() const { return height; }
	};
}