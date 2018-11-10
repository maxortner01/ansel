#pragma once

#include <Ansel.h>

using namespace Ansel;

namespace Game
{
	class Terrain : public Screen
	{
		const unsigned int CHUNK_DIMENSION = 600;
		const float size = .5f;

		Camera camera;
		
		std::vector<RawModel*> chunks;
		RawModel* water;
		Model* model;
		Shader* shader;

	public:
		Terrain(Window* w);

		float getNoise(const float x, const float y);
		RawModel* generateChunk(const int locx, const int locy, bool noise = true);

		void onUpdate() override;
		void onCreate() override;
	};
}