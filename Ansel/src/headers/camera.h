#pragma once

#include "Def.h"
#include "util.h"

namespace Ansel
{
	class Camera
	{
		mat4x4 view;
		vec3f location;

	public:
		ANSEL_API Camera();

		void ANSEL_API translate(vec3f vTranslate);
		void ANSEL_API translate(float x, float y, float z);

		mat4x4 ANSEL_API getView();
	};
}