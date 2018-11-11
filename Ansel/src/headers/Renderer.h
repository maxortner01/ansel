#pragma once

#include "Def.h"
#include "Model.h"
#include "Shader.h"
#include "camera.h"

namespace Ansel
{
	class Renderer
	{
		friend class Engine;

		struct RenderSettings
		{
			float _3D = 0.f;
			bool wireframe = false;
		};

		struct Light
		{
			vec4f location;
			vec3f color;
			bool  on;
		};

		static mat4x4 projection;

		static RenderSettings settings;
		static std::vector<Light> lights;

		static Shader* shader;
		static void prepare();
		static unsigned int uFrame;

	public:
		static mat4x4 ANSEL_API view;

		static void ANSEL_API init(vec2u dimensions);
		static void ANSEL_API loadShader(Shader* shader);

		static void ANSEL_API Render(Model* model);
		static void ANSEL_API Render(RawModel* rawModel, Camera camera, Shader* s = NULL);
		static void ANSEL_API Render(Model* model, Camera camera, Shader* s = NULL);
		static void ANSEL_API Render(std::vector<Model*> models, Camera camera = Camera(), Shader* s = NULL);

		static void ANSEL_API genProjection(float zNear, float zFar, float FOV, float aspectRatio);
		static void ANSEL_API set3D(bool isOn);
		static void ANSEL_API setWireFrame(bool isOn);
		static void ANSEL_API toggleWireFrame();
	};
}