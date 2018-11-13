#pragma once

#include "Def.h"
#include "Model.h"

namespace Ansel
{
	class ModelArray
	{
		friend class Renderer;

		std::vector<RawModel*> rawModels;
		std::vector<std::vector<Model*>> models;

	public:
		int ANSEL_API pushRawModel(RawModel* rawModel);
		int ANSEL_API pushModel(Model* model);

		RawModel ANSEL_API * getRawModel(int loc);

		void ANSEL_API destroyRawModel(RawModel* rawModel);
		void ANSEL_API destroyModel(Model* model);
	};
}