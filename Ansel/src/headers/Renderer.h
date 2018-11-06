#pragma once

#include "Def.h"
#include "Model.h"
#include "Shader.h"

#include <GL/glew.h>

namespace Ansel
{
	class Renderer
	{
		static Shader* shader;
		static void prepare();

	public:
		static void ANSEL_API init();
		static void ANSEL_API loadShader(Shader* shader);

		static void ANSEL_API Render(Model model);

	};
}