#pragma once

#include <Ansel.h>

using namespace Ansel;

namespace Game
{
	class Chunk
	{
		RawModel* land;
		RawModel* water;

		vec2f location;

	public:
		std::vector<Model*> trees;

		std::vector<vec4f> locations;
		std::vector<vec4f> rotations;
		std::vector<vec4f> scales;

		 Chunk(RawModel* l, RawModel* w, int x, int y);
		~Chunk();

		RawModel* getWaterModel() const;
		RawModel* getLandModel() const;
		vec2f getLocation() const;
	};

	class Player : public Camera
	{
	public:
		float velocity = 0.f;
	};

	class Terrain : public Screen
	{
		const int radius = 3;
		const unsigned int CHUNK_DIMENSION = 100;
		const float size = 30.f;
		const float terrainHeight = 75.f * 3.f;

		const float gravity = .25f;

		Player camera;
		
		Texture texture;
		std::vector<Chunk*> chunks;
		
		Shader* shader;

		RawModel* cylinder;
		void genCylinder();

	public:
		Terrain(Window* w);

		float getNoise(const float x, const float y);
		Chunk* generateChunk(int locx, int locy);

		void onUpdate() override;
		void onCreate() override;
	};
}