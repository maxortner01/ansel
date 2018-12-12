#include "../headers/Test2.h"

#include <iostream>

namespace Game
{
	Mover::Mover(Camera* camera) {
		_update = true;
		player = camera;
	}

	void Mover::update(ECS::EntityInstance entity) {
		Model* model = entity->getComponent("model")->cast<Model*>();
		Material* material = model->getRawModel()->getMaterial();

		float s = model->getScale().y;
		vec4f l = model->getLocation();

		//const float sin = sinf(Engine::getTime() / 1.2f + l.x);
		//const float cos = abs(cosf((Engine::getTime() / 2.f) + l.z + l.x)) * 2 - 1;
		const float noise = SimplexNoise::noise(l.x / 5.f, l.z / 5.f, Engine::getTime());

		model->setLocation(l.x, -s + noise / 5.f, l.z, 0);

		const float max_dist = 2.f;
		const float height_offset = .8f;
		const float max = -s + height_offset;
		const float min = -s - height_offset;
		const float dist = sqrtf(powf(l.x - player->getLocation().x, 2) + powf(l.z - player->getLocation().z, 2));
		if (dist < max_dist) {
			const float scale = powf(dist / max_dist, 2);
			const float color_grad = (1.f / ((max - (scale * (max - model->getLocation().y))) / max));
			model->setLocation(l.x, max - (scale * (max - model->getLocation().y)), l.z, 0);
			model->setColor({ 1 - color_grad, 1 - color_grad, 1 - color_grad, 1 });

			//if (material != nullptr)
			//	material->setEmissionStrength(1 - color_grad);
		}
		else {
			const float scale = powf(((-s + .25f) - (-s - .5f)) / (model->getLocation().y - (-s - .5f)), 2);
			model->setColor({ 1.f / scale, 1.f / scale, 1.f / scale, 1 });
			
			//if (material != nullptr)
			//	material->setEmissionStrength(1.f / scale);
		}

	}

	Test2::Test2(Window* w) : Screen(w) {}

	void Test2::onCreate() {
		Loader::makeRawModel("assets/models/cube.obj", "cube", false);

		std::vector<vec4f> colors;
		RawModel* rawModel = Loader::getRawModel("cube");
		for (int i = 0; i < rawModel->getVertexCount(); i++)
			colors.push_back({ 1, 1, 1, 1 });

		rawModel->loadColors(colors);

		Material* material = new Material;
		material->loadTexture(Material::ALBEDO, new Texture("assets/textures/rock/albedo.png"));
		material->loadTexture(Material::EMISSION, new Texture("assets/textures/emission.png"));
		//material->loadTexture(Material::NORMAL, new Texture("assets/textures/rock/normal.png"));

		material->setEmissionStrength(.25f);
		material->setEmissionColor({ 1.f, 1.f, 1.f, 1.f });

		material->setSpecStrength(32);
		material->setAmbientColor({ .01f, .01f, .01f, 1 });
		rawModel->loadMaterial(material);

		float width = .5f;
		float space = .05f;
		float height = 6.f;
		for (int z = 0; z < 30; z++) {
			for (int x = 0; x < 30; x++) {
				ECS::EntityInstance entity = Loader::makeEntity("floor_" + std::to_string(x) + "-" + std::to_string(z));

				Model* model = Loader::makeModel(Loader::getRawModel("cube"));
				entity->addComponent(model, "model");
				entity->addComponent(new Mover(&camera), "mover");

				model->setScale({ width / 2, height, width / 2});
				model->setLocation({ x * width + space * x, -height, z * width + space * z});
				
			}
		}

		Light light;
		light.on = true;
		light.type = light.DIRECTIONAL;
		light.location = { 0, 1, 1 };

		Renderer::makeLight(light);
	}

	void Test2::onUpdate(float timeDelta) {
		if (Keyboard::isKeyPressed(KEY::ESCAPE))
			window->close();

		float rotation_speed = .1f * 3.14159f / 180.f;
		vec2d newMouse = Mouse::getPostion();
		Mouse::setPosition({ ScreenSize.x / 2.f, ScreenSize.y / 2.f });

		vec2f change = { newMouse.x - ScreenSize.x / 2.f, newMouse.y - ScreenSize.y / 2.f };

		camera.rotate(0, -rotation_speed * change.x, 0);
		camera.rotate(rotation_speed * change.y, 0, 0);

		camera.setSpeed(.55f);
		camera.pollEvents();

		Renderer::Render(Loader::getEntities(), camera);

		std::cout << getFPS() << std::endl;
	}
}