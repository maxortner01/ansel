#pragma once

#include "../Def.h"
#include "../entities/Model.h"
#include "../entities/camera.h"

#include <ECS.h>

namespace Ansel
{
	class Skybox : public ECS::Entity
	{
		friend class Renderer;

		static RawModel* box;

		static class Mover : public ECS::Controller
		{
			Camera* player;

		public:
			Mover(Camera* camera);

			void update(ECS::EntityInstance entity);
		};

	public:
		ANSEL_API Skybox(Camera* camera);
	};
}