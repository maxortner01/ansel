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
		 Chunk(RawModel* l, RawModel* w, int x, int y);
		~Chunk();

		RawModel* getWaterModel() const;
		RawModel* getLandModel() const;
		vec2f getLocation() const;
	};

	class Terrain : public Screen
	{
		const int radius = 3;
		const unsigned int CHUNK_DIMENSION = 100;
		const float size = 9.f;
		const float terrainHeight = 75.f;

		Camera camera;
		
		std::vector<Chunk*> chunks;
		
		Shader* shader;

	public:
		Terrain(Window* w);

		float getNoise(const float x, const float y);
		Chunk* generateChunk(int locx, int locy);

		void onUpdate() override;
		void onCreate() override;
	};
}