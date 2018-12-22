#pragma once

#include <Ansel.h>

namespace Demo1 
{
	class Chunk : public ECS::Entity
	{
		Ansel::RawModel* _rawModel;

		static unsigned int count;
		unsigned int ID;

		static unsigned int width_amount;
		static float size;

		Ansel::vec2f realLocation;

	public:
		static float getTerrain(Ansel::vec2f location);

		static void genChunk(Ansel::ModelContainer* container, Ansel::vec2i chunk_loc);

		Chunk(Ansel::vec2i chunk_loc);
		Chunk(Ansel::RawModel* rawModel);
		~Chunk();
	};
}