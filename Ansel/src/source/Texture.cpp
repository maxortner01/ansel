#include "../headers/Texture.h"

#include "../vendor/stb_image.h"
#include <GL/glew.h>

namespace Ansel
{
	Texture::Texture() {

	}

	Texture::Texture(const int w, const int h, FrameBufferType type) {
		// Initialize variables
		ID = 0;
		localBuffer = nullptr;
		width = w; height = h; BBP = 0;

		// Create texture in OpenGL
		glGenTextures(1, &ID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLenum i = GL_RGB;
		GLenum t = GL_RGB;

		switch (type) {
			case DEPTH:
				i = GL_DEPTH_COMPONENT24;
				t = GL_DEPTH_COMPONENT;

			default:
				break;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, i, width, height, 0, t, GL_UNSIGNED_BYTE, NULL);
			
		unBind();
	}

	Texture::Texture(const std::string &path) {
		// Initialize variables
		ID = 0;
		filename = path;
		localBuffer = nullptr;
		width = 0; height = 0; BBP = 0;

		const char* cstring = path.c_str();
		
		// Create texture in OpenGL
		glGenTextures(1, &ID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ID);

		// Load image
		stbi_set_flip_vertically_on_load(true);
		localBuffer = stbi_load(cstring, &width, &height, &BBP, STBI_rgb_alpha);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (localBuffer) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
			int e = glGetError();

			glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(localBuffer);
		}
		else {
			/* error loading image */
		}

		unBind();
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