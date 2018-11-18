#include "../headers/Textures.h"

namespace Game
{
	Textures::Textures(Window* w) : Screen(w) {
		texture = Texture("assets/textures/grass.png");

		std::vector<vec3f> vertices = {
			{ -.5f, -.5f, 1.f },
			{  .5f, -.5f, 1.f },
			{  .5f,  .5f, 1.f },
			{ -.5f,  .5f, 1.f }
		};

		std::vector<vec2f> tex = {
			{ 0, 0 },
			{ 1, 0 },
			{ 1, 1 },
			{ 0, 1 }
		};

		std::vector<unsigned int> indices = {
			0, 1, 2,
			2, 3, 0
		};

		square = Loader::makeRawModel(vertices, indices);
		square->loadTexture(&texture);
		square->loadTextureCoordinates(tex);
	}

	void Textures::onCreate() {

	}

	void Textures::onUpdate() {
		Renderer::Render(square);
	}
}