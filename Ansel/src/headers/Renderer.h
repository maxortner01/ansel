#pragma once

#include "Def.h"
#include "Model.h"
#include "Shader.h"
#include "camera.h"

namespace Ansel
{
	class Renderer
	{
		struct RenderSettings
		{
			float _3D = 0.f;
		};

		static mat4x4 projection;

		static RenderSettings settings;

		static Shader* shader;
		static void prepare();
		static unsigned int uFrame;

	public:
		static mat4x4 ANSEL_API view;

		static void ANSEL_API init(vec2u dimensions);
		static void ANSEL_API loadShader(Shader* shader);

		static void ANSEL_API Render(Model model);
		static void ANSEL_API Render(std::vector<Model*> models, Camera camera = Camera());

		static void ANSEL_API genProjection(float zNear, float zFar, float FOV, float aspectRatio);
		static void ANSEL_API set3D(bool isOn);
	};
}