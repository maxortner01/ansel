#include "../headers/Texture.h"

#include "../vendor/stb_image.h"
#include <GL/glew.h>

namespace Ansel
{
	Texture::Texture() {

	}

	Texture::Texture(const std::string &path) {
		// Initialize variables
		ID = 0;
		filename = path;
		localBuffer = nullptr;
		width = 0; height = 0; BBP = 0;

		const char* cstring = path.c_str();

		// Load image
		stbi_set_flip_vertically_on_load(true);
		localBuffer = stbi_load(cstring, &width, &height, &BBP, STBI_rgb_alpha);

		// Create texture in OpenGL
		glGenTextures(1, &ID);
		bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
		int err = glGetError();

		unBind();

		if (localBuffer)
			stbi_image_free(localBuffer);
	}

	Texture::~Texture() {
		glDeleteTextures(1, &ID);
	}

	void Texture::bind(unsigned int slot) const {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, ID);
	}

	void Texture::unBind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}