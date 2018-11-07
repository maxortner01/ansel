#include "../headers/camera.h"

namespace Ansel
{
	Camera::Camera() {
		location = { 0, 0, 0 };
	}

	void Camera::translate(vec3f vTranslate) {
		location = {
			location.x + vTranslate.x,
			location.y + vTranslate.y,
			location.z + vTranslate.z
		};
	}

	void Camera::translate(float x, float y, float z) {
		translate({ x, y, z });
	}

	mat4x4 Camera::getView() {
		view = mat4x4();

		view.setTranslation(-location.x, -location.y, -location.z);

		return view;
	}
}