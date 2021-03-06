#include "../../headers/rendering/Material.h"

namespace Ansel
{
	Material::Material() {
		textures.resize(AMOUNT);
	}

	Material::~Material() {
		for (int i = 0; i < AMOUNT; i++) {
			delete textures.at(i);
		}

		textures.clear();
	}

	void Material::loadShader(Shader* s) {
		shader = s;
	}

	Shader* Material::getShader() const {
		return shader;
	}

	void Material::loadTexture(int type, Texture* texture) {
		textures.at(type) = texture;
	}

	Texture* Material::getTexture(int type) const {
		return textures.at(type);
	}
}