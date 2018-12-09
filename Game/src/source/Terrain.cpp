#include "../headers/Terrain.h"

namespace Game
{
	Terrain::Terrain(Window* w) : Screen(w) {}

	void Terrain::onCreate() {
		RawModel* cube = Loader::makeRawModel("assets/models/cube.obj", "cube");

		ECS::EntityInstance entity = Loader::makeEntity("cube");
		entity->addComponent(Loader::makeModel(Loader::getRawModel("cube")), "model");
		
		std::vector<vec4f> colors;
		for (int i = 0; i < cube->getVertexCount(); i++)
			colors.push_back({ 1, 1, 1, 1 });

		cube->loadColors(colors);

		Renderer::setLighting(false);
	}

	void Terrain::onUpdate(float timeDelta) {
		camera.setSpeed(.25f);
		camera.pollEvents();
		Renderer::Render(Loader::getEntities(), camera);
		
	}
}