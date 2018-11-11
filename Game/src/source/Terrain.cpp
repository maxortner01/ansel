#include "../headers/Terrain.h"

namespace Game
{
	Chunk::Chunk(RawModel* l, RawModel* w, int x, int y) {
		water = w; land = l;

		location = { (float)x, (float)y };
	}

	Chunk::~Chunk() {
		delete water;
		delete land;
	}

	RawModel* Chunk::getWaterModel() const {
		return water;
	}

	RawModel* Chunk::getLandModel() const {
		return land;
	}

	vec2f Chunk::getLocation() const {
		return location;
	}

	Terrain::Terrain(Window* w) : Screen(w) {
		
	}

	void Terrain::onCreate() {
		chunks.push_back(generateChunk(0, 0));

		shader = new Shader("assets/shaders/shader.vert", "assets/shaders/shader.frag", false);
		shader->makeShader("assets/shaders/water.geo", GEOMETRY);

		shader->link();

		camera.setLocation({ -89.67f, 0.f, 384.11f });
	}

	Chunk* Terrain::generateChunk(int locx, int locy) {
		std::vector<vec3f> vertices, waterv;
		std::vector<unsigned int> indices;

		const int final_x = locx;
		const int final_y = locy;

		locx = locx * CHUNK_DIMENSION;
		locy = locy * CHUNK_DIMENSION;

		for (int y = 0; y <= CHUNK_DIMENSION; y++) {
			for (int x = 0; x <= CHUNK_DIMENSION; x++) {
				// If the current position is one less than the current dimension
				// calculate the indices for the squares
				if (y != CHUNK_DIMENSION && x != CHUNK_DIMENSION) {
					indices.push_back(y * (CHUNK_DIMENSION + 1)+x);
					indices.push_back((y + 1) * (CHUNK_DIMENSION + 1)+x + 1);
					indices.push_back(y * (CHUNK_DIMENSION + 1)+x + 1);

					indices.push_back((y + 1) * (CHUNK_DIMENSION + 1)+x);
					indices.push_back((y + 1) * (CHUNK_DIMENSION + 1)+x + 1);
					indices.push_back(y * (CHUNK_DIMENSION + 1)+x);
				}
				
				// Also calculate the point
				int X = x + locx; int Y = y + locy;

				float n = getNoise((float)X * size, (float)Y * size);

				vertices.push_back({ (float)X * size, n, (float)Y * size });
				waterv.push_back({ (float)X * size, -.8f * terrainHeight, (float)Y * size });
			}
		}

		RawModel* water;
		RawModel* land;
		land  = Loader::makeRawModel(vertices, indices);
		water = Loader::makeRawModel(waterv, indices);

		return new Chunk(land, water, final_x, final_y);
	}

	float Terrain::getNoise(const float x, const float y) {
		std::unique_ptr<SimplexNoise> n = std::make_unique<SimplexNoise>();

		float v = n->fractal(4, x / (4000.f), y / (4000.f));
		float v2 = n->fractal(6, x / (400.f), y / (400.f)) / 2.f;

		v  = 1 - abs(v);
		v -= 1; v *= 2;

		v = (((v + powf(v2, 2)) / 2.f) - 1) * 2 + 2;

		//v = -abs(v) + 1;

		//v = powf(v, 2) - 1;

		return v * terrainHeight;
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

		if (Keyboard::isKeyPressed(KEY::TAB))
			Renderer::toggleWireFrame();

		vec2d newMouse = Mouse::getPostion();
		Mouse::setPosition({ ScreenSize.x / 2.f, ScreenSize.y / 2.f });

		vec2f change = { newMouse.x - ScreenSize.x / 2.f, newMouse.y - ScreenSize.y / 2.f };

		camera.rotate(0, -rotation_speed * change.x, 0);
		camera.rotate(rotation_speed * change.y, 0, 0);

		float chunk_size = (int)CHUNK_DIMENSION * size;
		vec2i camera_chunk_location = { (int)floor(camera.getLocation().x / chunk_size), (int)floor(camera.getLocation().z / chunk_size) };

		camera.setLocation({
			camera.getLocation().x,
			getNoise(camera.getLocation().x, camera.getLocation().z) + 4.f,
			camera.getLocation().z
		});

		for (int y = -1; y < 2; y++) {
			for (int x = -1; x < 2; x++) {
				int X = x + camera_chunk_location.x;
				int Y = y + camera_chunk_location.y;

				bool found = false;
				for (int i = 0; i < chunks.size(); i++) {
					Chunk* c = chunks.at(i);

					if (c->getLocation().x == X && c->getLocation().y == Y) {
						found = true; break;
					}
					else if (c->getLocation().x > X + 2 || c->getLocation().y > Y + 2 ) {
						chunks.erase(chunks.begin() + i);
						continue;
					}
					else if (c->getLocation().x < X - 2  || c->getLocation().y < Y - 2 ) {
						chunks.erase(chunks.begin() + i);
						continue;
					}
				}

				if (!found) chunks.push_back(generateChunk(X, Y));
			}
		}

		for (int i = 0; i < chunks.size(); i++) {
			Chunk* chunk = chunks.at(i);

			Renderer::Render(chunk->getLandModel(), camera);
		}

		for (int i = 0; i < chunks.size(); i++) {
			Chunk* chunk = chunks.at(i);

			Renderer::Render(chunk->getWaterModel(), camera, shader);
		}

		printf((std::to_string(getFPS()) + "\n").c_str());
		uFrame++;
	}
}