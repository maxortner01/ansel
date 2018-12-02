#pragma once

#include <Ansel.h>

using namespace Ansel;

namespace Game
{
	class Textures : public Screen 
	{
		Texture*  texture;
		RawModel* square;
		Model* squareModel;

		RawModel* background;
		Model* bgmodel;

	public:
		Textures(Window* w);

		void onUpdate() override;
		void onCreate() override;
	};
}