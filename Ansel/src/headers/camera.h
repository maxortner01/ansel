#pragma once

#include "Def.h"
#include "util.h"

namespace Ansel
{
	class Camera
	{
		vec3f location;
		vec3f rotation;

	public:
		enum DIRECTION {
			FORWARDS,
			BACKWARDS,
			LEFT,
			RIGHT
		};

		ANSEL_API Camera();

		void ANSEL_API translate(vec3f vTranslate);
		void ANSEL_API translate(float x, float y, float z);

		void ANSEL_API rotate(vec3f vRotate);
		void ANSEL_API rotate(float x, float y, float z);

		void ANSEL_API move(DIRECTION direction, float speed);

		mat4x4 ANSEL_API getView();
		vec3f  ANSEL_API getLocation();

		void  ANSEL_API setLocation(vec3f loc);
	};
}