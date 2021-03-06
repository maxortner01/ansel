#include "../headers/MainScreen.h"

namespace Game
{
	Asteroid::Asteroid(RawModel* rawModel, const int max, const int min) : Model(rawModel) {

		rotation_rates = { 
			(rand() % 360000) / 100000.f * 3.14159f / 180.f,
			(rand() % 360000) / 100000.f * 3.14159f / 180.f,
			(rand() % 360000) / 100000.f * 3.14159f / 180.f
		};

		angle = (rand() % 360000) / 1000.f;
		radius = (rand() % (max - min)) + min;
		
		float scale_factor = (rand() % 10) / 200.f;
		setScale({ scale_factor, scale_factor, scale_factor });
	}

	void Asteroid::update() {
		setLocation(radius * cosf(angle * 180.f / 3.14159 / radius), 0, radius * sinf(angle * 180.f / 3.14159 / radius), 1);
		angle += .001f;

		addRotation(rotation_rates.x, rotation_rates.y, rotation_rates.z);
	}

	MainScreen::MainScreen(Window* window) : Screen(window) {
		std::vector<vec3f> locations = {
			{ -1.f,  1.f, 1.f },
			{ -1.f, -1.f, 1.f },
			{  1.f, -1.f, 1.f },
			{  1.f,  1.f, 1.f },

			{ -1.f,  1.f, -1.f },
			{ -1.f, -1.f, -1.f },
			{  1.f, -1.f, -1.f },
			{  1.f,  1.f, -1.f },
		};

		std::vector<vec4f> colors = {
			{ 0, 0, 1, 1 },
			{ 1, 0, 0, 1 },
			{ 0, 1, 0, 1 },
			{ 1, 1, 1, 1 },

			{ 0, 0, 1, 1 },
			{ 1, 0, 0, 1 },
			{ 0, 1, 0, 1 },
			{ 1, 1, 1, 1 },
		};

		std::vector<unsigned int> indices = {
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4,

			3, 2, 6,
			6, 7, 3,

			0, 1, 5,
			5, 4, 0,

			0, 4, 7,
			7, 3, 0,

			1, 5, 6,
			6, 2, 1
		};


		rawModel = Loader::makeRawModel("assets/models/Asteroid.obj");
		rawPlanet = Loader::makeRawModel("assets/models/sphere.obj");

		colors.resize(rawModel->getVertexCount());
		for (int i = 0; i < rawModel->getVertexCount(); i++)
			colors.at(i) = { 1, 1, 1, 1 };

		rawModel->loadColors(colors);

		colors.resize(rawPlanet->getVertexCount());
		for (int i = 0; i < rawPlanet->getVertexCount(); i++)
			colors.at(i) = { 1, 1, 1, 1 };

		rawPlanet->loadColors(colors);

		int range = 1;
		for (int y = 0; y < range; y++) {
			for (int x = 0; x < range; x++) {
				Model* model = new Asteroid(rawModel);
		
				models.push_back(model);
			}
		}

		Model* model = new Asteroid(rawModel);
		models.push_back(model);

		planetModel = new Model(rawPlanet);
		planetModel->setScale({ 5, 5, 5 });

		Mouse::hideCursor();
	}

	void MainScreen::onUpdate() {
		if (Keyboard::isKeyPressed(KEY::ESCAPE))
			Window::close();

		float rotation_speed = .1f * 3.14159f / 180.f;
		float speed = .1f;

		if (Keyboard::isKeyPressed(KEY::L_SHIFT))
			speed *= 2;

		if (Keyboard::isKeyPressed(KEY::W))
			camera.move(Camera::FORWARDS, speed);

		if (Keyboard::isKeyPressed(KEY::S))
			camera.move(Camera::BACKWARDS, speed);

		if (Keyboard::isKeyPressed(KEY::D))
			camera.move(Camera::RIGHT, speed);

		if (Keyboard::isKeyPressed(KEY::A))
			camera.move(Camera::LEFT, speed);

		if (Keyboard::isKeyPressed(KEY::SPACE))
			camera.translate(0, speed, 0);

		if (Keyboard::isKeyPressed(KEY::L_CTRL))
			camera.translate(0, -speed, 0);

		if (Keyboard::isKeyPressed(KEY::UP))
			camera.rotate(-rotation_speed, 0, 0);

		if (Keyboard::isKeyPressed(KEY::DOWN))
			camera.rotate(rotation_speed, 0, 0);

		if (Keyboard::isKeyPressed(KEY::LEFT))
			camera.rotate(0, rotation_speed, 0);

		if (Keyboard::isKeyPressed(KEY::RIGHT))
			camera.rotate(0, -rotation_speed, 0);

		if (Keyboard::isKeyPressed(KEY::TAB))
			Renderer::toggleWireFrame();

		vec2d newMouse = Mouse::getPostion();
		Mouse::setPosition({ ScreenSize.x / 2.f, ScreenSize.y / 2.f });

		vec2f change = { newMouse.x - ScreenSize.x / 2.f, newMouse.y - ScreenSize.y / 2.f };
		
		camera.rotate(0, -rotation_speed * change.x, 0);
		camera.rotate(rotation_speed * change.y, 0, 0);

		for (int i = 0; i < models.size(); i++)
			models.at(i)->update();

		Renderer::Render(models, camera);
		Renderer::Render(planetModel, camera);

		system("CLS");
		std::cout << getFPS() << std::endl;
		//printf((std::to_string(getFPS()) + "\n").c_str());

		uFrame++;
	}

	void MainScreen::onDestroy() {

	}
}