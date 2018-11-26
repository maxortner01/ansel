#pragma once

#include <Ansel.h>
#include <vector>

using namespace Ansel;

namespace Game
{
	class LightTest : public Screen
	{
		RawModel* rawFloor;
		RawModel* cylinder;
		RawModel* rawStone;
		RawModel* treemodel;
		
		Model* box;
		Model* tree;
		Model* realTree;
		std::vector<Model*> floor;

		StaticModelArray sma;
		ParticleSystem* part;

		bool   update = true;
		Camera camera;

	public:
		LightTest(Window* w);

		void onCreate() override;
		void onUpdate() override;
	};


}