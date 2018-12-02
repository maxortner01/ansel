#include "../headers/LightTest.h"

#include <cstdlib>
#include <iostream>
#include <fstream>

namespace Game
{
	LightTest::LightTest(Window* w) : Screen(w) {}

	void LightTest::onCreate() {
		/* <---------- TREE ----------> */

		cylinder = Loader::makeRawModel("assets/models/cylinder.obj", false);

		Material* material = new Material;
		material->loadTexture(Material::ALBEDO, new Texture("assets/textures/bark/albedo.png"));
		material->loadTexture(Material::NORMAL, new Texture("assets/textures/bark/normal.png"));
		material->loadTexture(Material::OCCLUSION, new Texture("assets/textures/bark/ao.png"));

		cylinder->loadMaterial(material);

		tree = new Model(cylinder);
		tree->setLocation({ 3.f, 1.f, 0 });

		/* <---------- CUBE ----------> */

		rawStone = Loader::makeRawModel("assets/models/cube.obj", false);

		material = new Material;
		material->loadTexture(Material::ALBEDO, new Texture("assets/textures/rock/albedo.png"));
		material->loadTexture(Material::NORMAL, new Texture("assets/textures/rock/normal.png"));
		material->loadTexture(Material::OCCLUSION, new Texture("assets/textures/rock/ao.png"));

		rawStone->loadMaterial(material);

		box = new Model(rawStone);
		box->setLocation({-3.f, .5f, 0});

		/* <---------- FLOOR ----------> */

		std::vector<vec3f> vertices = {
			{ -1.f,  0.f,  1.f },
			{  1.f,  0.f,  1.f },
			{  1.f,  0.f, -1.f },
			{ -1.f,  0.f, -1.f }
		};

		std::vector<unsigned int> indices = {
			0, 1, 2,
			2, 3, 0
		};

		std::vector<vec2f> tex = {
			{ 0.f, 1.f },
			{ 1.f, 1.f },
			{ 1.f, 0.f },
			{ 0.f, 0.f }
		};

		rawFloor = Loader::makeRawModel(vertices, indices);
		rawFloor->loadTextureCoordinates(tex);
		
		material = new Material;
		material->loadTexture(Material::ALBEDO, new Texture("assets/textures/dirt/albedo.png"));
		material->loadTexture(Material::NORMAL, new Texture("assets/textures/dirt/normal.png"));
		material->loadTexture(Material::OCCLUSION, new Texture("assets/textures/dirt/ao.png"));

		rawFloor->loadMaterial(material);

		int l = 5;
		for (int z = -1 - l; z < 2 + l; z++) {
			for (int x = -1 - l; x < 2 + l; x++) {
				Model* model = new Model(rawFloor);

				model->setLocation({ (float)x * 2, 0, (float)z * 2 });

				floor.push_back(model);
			}
		}

		/* <---------- EVERYTHING ELSE ----------> */

		//Renderer::setLighting(false);
		camera.setSpeed(2.f);

		Light light;

		light.on = true;
		//light.location = { 1, .65f, 1 };
		light.location = { 1, 1, 0 };
		light.type = light.DIRECTIONAL;
		light.color = { 1, 1, 1 };

		Renderer::makeLight(light);

		PSys::ParticleProperties partProp;
		partProp.gravity = .2f;
		partProp.life = 4.f;
		partProp.rate = 100;
		//partProp.birth_color = { .1f, .25f, .8f, 1 };
		partProp.birth_color = { 1, .25f, 0, 1 };
		partProp.death_color = { 0, 0, 0, 0 };
		partProp.initial_velocity = .0005f;
		
		part = new ParticleSystem({ 0, 0, 0 });
		part->setProperties(partProp);
		part->setEmitterType(ParticleSystem::POINT);
		part->setPlane({ 0, 0, .5f, .5f });
		part->setHeight(.9f);

		part->setLocation({ 0, 2.f, 0 });
		part->set();

		Mouse::hideCursor();

		healthbar = new Texture("assets/textures/health.png");

		font = new Font("assets/fonts/Arial.ttf");
		text = new Text(font);
		text->setString("what up");
	}

	void LightTest::onUpdate() {
		if (Keyboard::isKeyPressed(KEY::ESCAPE))
			window->close();

		if (Keyboard::isKeyPressed(KEY::LEFT))
			update = false;

		if (Keyboard::isKeyPressed(KEY::RIGHT))
			update = true;

		camera.setSpeed(5.f);
		camera.pollEvents();

		float rotation_speed = .1f * 3.14159f / 180.f;
		vec2d newMouse = Mouse::getPostion();
		Mouse::setPosition({ ScreenSize.x / 2.f, ScreenSize.y / 2.f });

		vec2f change = { newMouse.x - ScreenSize.x / 2.f, newMouse.y - ScreenSize.y / 2.f };

		//Light* l = Renderer::getLight(0);
		//l->location = { cosf((float)Engine::getTime()), 0, sinf((float)Engine::getTime()) };

		camera.rotate(0, -rotation_speed * change.x, 0);
		camera.rotate(rotation_speed * change.y, 0, 0);

		//part->setLocation({ 5.f * sinf((float)Engine::getTime() * 25.0), 5.f * cosf((float)Engine::getTime() * 25.0), 0 });
		part->setLocation({ 1.f * sinf((float)Engine::getTime() * 5.0), 3.f, 1.f * cosf((float)Engine::getTime() * 5.0) });
		
		if (update)
			part->update();

		Renderer::Render(floor, camera);
		Renderer::Render(tree, camera);
		Renderer::Render(box, camera);

		//Renderer::Render(text, camera);
		Renderer::Render(part, camera);

		//std::cout << part->getParticleSize() << std::endl;
		std::cout << getFPS() << std::endl;
	}

	void LightTest::renderUI(float aspectRatio) {
		draw.aspectRatio = aspectRatio;
		draw.circle({ 0, 0 }, .005f, 25);

		//Renderer::Render(text, Camera(), Renderer::getFrameShader());

		//draw.texture({ 1.f - .5f, -1.f + .25f, 1.f / 1.5f, .6f / 1.5f }, healthbar);
	}
}