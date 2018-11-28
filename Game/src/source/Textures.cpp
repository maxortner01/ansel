#include "../headers/Textures.h"

namespace Game
{
	Textures::Textures(Window* w) : Screen(w) {
		
	}

	void Textures::onCreate() {
		Renderer::setLighting(false);

		texture = new Texture("assets/textures/awesomeface.png");

		std::vector<vec3f> vertices = {
			{ -.5f, -.5f, 0.f },
			{  .5f, -.5f, 0.f },
			{  .5f,  .5f, 0.f },
			{ -.5f,  .5f, 0.f }
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

		std::vector<vec4f> colors = {
			{ .2f, .2f, .5f, 1.f },
			{ .2f, .2f, .5f, 1.f },
			{ .2f, .2f, .5f, 1.f },
			{ .2f, .2f, .5f, 1.f },
		};

		square = Loader::makeRawModel(vertices, indices);
		square->loadTexture(texture);
		square->loadTextureCoordinates(tex);

		background = Loader::makeRawModel(vertices, indices);
		background->loadColors(colors);

		bgmodel = Loader::makeModel(background);
		bgmodel->setScale({ 10, 10, 1 });
		bgmodel->setLocation({ 0, 0, 2.f });

		squareModel = Loader::makeModel(square);
		squareModel->setLocation(0, 0, 1, 1);
	}

	void Textures::onUpdate() {
		squareModel->addRotation(0, .01f, 0);
		
		Renderer::Render(bgmodel);
		Renderer::Render(squareModel);

		float fps = getFPS();
	}
}