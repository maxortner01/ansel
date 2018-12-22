#include "../../headers/entities/Skybox.h"

namespace Ansel
{
	RawModel* Skybox::box = nullptr;

	Skybox::Mover::Mover(Camera* camera) {
		player = camera;
	}

	void Skybox::Mover::update(ECS::EntityInstance entity) {
		Model* model = entity->getComponent("model")->cast<Model*>();
		model->setLocation(player->getLocation().x, player->getLocation().y, player->getLocation().z, 1);
	}

	Skybox::Skybox(Camera* camera) {
		Model* model = Loader::makeModel(Skybox::box);
		model->setScale({ 750.f, 750.f, 750.f });

		addComponent(new Mover(camera), "mover");
		addComponent(model, "model");
	}
}