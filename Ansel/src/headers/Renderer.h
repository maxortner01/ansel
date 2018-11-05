#pragma once

#include "Def.h"

#include "Model.h"
#include <GL/glew.h>

namespace Ansel
{
	class ANSEL_API Renderer
	{
		static void prepare();

	public:
		static void Render(Model model);

	};
}