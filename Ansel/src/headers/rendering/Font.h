#pragma once

#include "../Def.h"
#include "../util.h"

#include "../rendering/Shader.h"

#include <map>

namespace Ansel
{
	struct Character {
		unsigned int TextureID;
		vec2f		 Size;
		vec2f		 Bearing;
		unsigned int Advance;
		void*        tex_ptr;
	};

	class Font
	{
		friend class Renderer;

		static Shader* shader;

		std::map<char, Character> Characters;

		// size of the characters
		unsigned int character_size;

	public:
		ANSEL_API  Font();
		ANSEL_API  Font(const char* filename, unsigned int char_size = 24);
		ANSEL_API ~Font();

		void ANSEL_API load(const char* filename);

		void ANSEL_API setCharacterSize(const unsigned int c);
		std::map<char, Character> ANSEL_API getCharacters() const;
	};
}