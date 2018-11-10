#include "../headers/Terrain.h"

namespace Game
{
	Terrain::Terrain(Window* w) : Screen(w) {
		
	}

	void Terrain::onCreate() {
		chunks.push_back(generateChunk(0, 0));
		water = generateChunk(0, 0, false);

		shader = new Shader("assets/shaders/shader.vert", "assets/shaders/shader.frag", false);
		shader->makeShader("assets/shaders/water.geo", GEOMETRY);

		shader->link();
	}

	RawModel* Terrain::generateChunk(const int locx, const int locy, bool noise) {
		std::vector<vec3f> vertices;
		std::vector<unsigned int> indices;

		for (int y = 0; y < CHUNK_DIMENSION; y++) {
			for (int x = 0; x < CHUNK_DIMENSION; x++) {
				float n = -.8 * 12.f;
				if (noise)
					n = getNoise((float)x * size, (float)y * size);

				vertices.push_back({ (float)x * size, n, (float)y * size });
			}
		}

		for (int y = 0; y < CHUNK_DIMENSION - 1; y++) {
			for (int x = 0; x < CHUNK_DIMENSION - 1; x++) {
				indices.push_back(y * (CHUNK_DIMENSION) + x);
				indices.push_back((y + 1) * (CHUNK_DIMENSION) + x + 1);
				indices.push_back(y * (CHUNK_DIMENSION)+x + 1);

				indices.push_back((y + 1) * (CHUNK_DIMENSION) + x);
				indices.push_back((y + 1) * (CHUNK_DIMENSION) + x + 1);
				indices.push_back(y * (CHUNK_DIMENSION) + x);
			}
		}

		return Loader::makeRawModel(vertices, indices);
	}

	float Terrain::getNoise(const float x, const float y) {
		std::unique_ptr<SimplexNoise> n = std::make_unique<SimplexNoise>();

		float v = n->fractal(7, x / 200.f, y / 200.f);

		v = -abs(v) + 1;
		v -= 1; v *= 2;

		return v * 12.f;
	}

	void Terrain::onUpdate() {
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

		vec2d newMouse = Mouse::getPostion();
		Mouse::setPosition({ ScreenSize.x / 2.f, ScreenSize.y / 2.f });

		vec2f change = { newMouse.x - ScreenSize.x / 2.f, newMouse.y - ScreenSize.y / 2.f };

		camera.rotate(0, -rotation_speed * change.x, 0);
		camera.rotate(rotation_speed * change.y, 0, 0);

		for (int i = 0; i < chunks.size(); i++) {
			Renderer::Render(chunks.at(i), camera);
		}

		Renderer::Render(water, camera, shader);
		
		printf((std::to_string(getFPS()) + "\n").c_str());
	}
}