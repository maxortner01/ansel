#include "../../headers/rendering/Font.h"
#include "../../headers/rendering/Texture.h"

#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "GL/glew.h"

namespace Ansel
{
	Font::Font() {

	}

	Font::Font(const char* filename, unsigned int char_size) {
		setCharacterSize(char_size);
		load(filename);
	}

	Font::~Font() {

	}

	void Font::load(const char* filename) {
		FT_Library library;
		if (FT_Init_FreeType(&library))
			std::cout << "ERROR: Could not init freetype library.\n";

		FT_Face face;
		if (FT_New_Face(library, filename, 0, &face))
			std::cout << "ERROR: Failed to load font: " << filename << "\n";

		FT_Set_Pixel_Sizes(face, 0, character_size);

		if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
			std::cout << "ERROR: Failed to load glyph.\n";

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		for (unsigned char c = 0; c < 128; c++) {

			// Load the character glyph
			if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
				std::cout << "WARNING: Failed to load glyph: " << c << "\n";
				continue;
			}

			// Generate texture
			Texture* texture = new Texture(face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer);
			Character character = {
				texture->getID(),
				{ face->glyph->bitmap.width, face->glyph->bitmap.rows },
				{ face->glyph->bitmap_left , face->glyph->bitmap_top  },
				face->glyph->advance.x,
				texture
			};
			Characters.insert(std::pair<char, Character>(c, character));
		}
	}

	void Font::setCharacterSize(const unsigned int c) {
		character_size = c;
	}

	std::map<char, Character> Font::getCharacters() const {
		return Characters;
	}
}