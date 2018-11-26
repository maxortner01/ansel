#pragma once

#include "Def.h"
#include "Texture.h"

#include <vector>

namespace Ansel
{
	class Material
	{
		std::vector<Texture*> textures;

	public:
		enum TEXTURES {
			ALBEDO,
			NORMAL,
			OCCLUSION,
			AMOUNT
		};

		ANSEL_API Material();
		ANSEL_API ~Material();

		void ANSEL_API loadTexture(int type, Texture* texture);
		Texture ANSEL_API * getTexture(int type) const;
	};
}