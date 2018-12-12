#pragma once

#include "../Def.h"

#include "Texture.h"
#include "Shader.h"

#include <vector>

namespace Ansel
{
	class Material
	{
		std::vector<Texture*> textures;
		Shader* shader = nullptr;

		float specStrength = 0.f;
		float emissionStrength = 0.f;

		vec4f emissionColor;
		vec4f ambientColor;

	public:
		enum TEXTURES {
			ALBEDO,
			NORMAL,
			OCCLUSION,
			EMISSION,
			AMOUNT
		};

		ANSEL_API Material();
		ANSEL_API ~Material();

		void ANSEL_API loadShader(Shader* s);
		Shader ANSEL_API * getShader() const;

		void ANSEL_API loadTexture(int type, Texture* texture);
		Texture ANSEL_API * getTexture(int type) const;

		void  setSpecStrength(float spec) { specStrength = spec; }
		float getSpecStrength() { return specStrength; }

		void  setEmissionStrength(float em) { emissionStrength = em; }
		float getEmissionStrength() { return emissionStrength; }

		void  setAmbientColor(vec4f col) { ambientColor = col; }
		vec4f getAmbientColor() { return ambientColor; }

		void  setEmissionColor(vec4f col) { emissionColor = col; }
		vec4f getEmissionColor() { return emissionColor; }

		unsigned int getTextureSize() const { return textures.size(); }
	};
}