#pragma once

#include "../Def.h"
#include "../util.h"

#include "Model.h"

#include <vector>

namespace Ansel
{
	class StaticModelArray
	{
		std::vector<vec4f>  locations, rotations, scales;
		std::vector<Model*> models;

	public:
		enum TRANSFORMATION
		{
			LOCATIONS,
			ROTATION,
			SCALES
		} typedef TRANSFORMATION;

		ANSEL_API StaticModelArray();
		ANSEL_API StaticModelArray(std::vector<Model*> m);

		void ANSEL_API addModel(Model* model);
		void ANSEL_API removeModel(Model* model);

		std::vector<Model*> ANSEL_API getModels() const;
		std::vector<vec4f> ANSEL_API getTransformation(TRANSFORMATION transformation) const;

		void ANSEL_API bake();
	};
}