#include "../../headers/entities/camera.h"

#include "../../headers/event/Keyboard.h"

namespace Ansel
{
	Camera::Camera() {
		location = { 0, 0, 0 };
		rotation = { 0, 0, 0 };
	}

	void Camera::pollEvents() {
		float s = speed;
		if (Keyboard::isKeyPressed(KEY::L_SHIFT))
			s *= 2;

		if (Keyboard::isKeyPressed(KEY::W))
			move(Camera::FORWARDS, s);

		if (Keyboard::isKeyPressed(KEY::S))
			move(Camera::BACKWARDS, s);

		if (Keyboard::isKeyPressed(KEY::D))
			move(Camera::RIGHT, s);

		if (Keyboard::isKeyPressed(KEY::A))
			move(Camera::LEFT, s);

		if (Keyboard::isKeyPressed(KEY::SPACE))
			translate(0, s, 0);

		if (Keyboard::isKeyPressed(KEY::L_CTRL))
			translate(0, -s, 0);
	}

	void Camera::setSpeed(float s) {
		speed = s;
	}

	void Camera::move(DIRECTION direction, float speed) {
		switch (direction) {
		case FORWARDS:
			translate(speed * cosf(rotation.y + 3.14159f / 2.f), 0, speed * sinf(rotation.y + 3.14159f / 2.f));
			break;

		case BACKWARDS:
			translate(-speed * cosf(rotation.y + 3.14159f / 2.f), 0, -speed * sinf(rotation.y + 3.14159f / 2.f));
			break;

		case LEFT:
			translate(-speed * cosf(rotation.y), 0, -speed * sinf(rotation.y));
			break;

		case RIGHT:
			translate(speed * cosf(rotation.y), 0, speed * sinf(rotation.y));
			break;

		default:
			return;
		};
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

	void Camera::rotate(vec3f vRotate) {
		rotation = {
			rotation.x + vRotate.x,
			rotation.y + vRotate.y,
			rotation.z + vRotate.z
		};
	}

	void Camera::rotate(float x, float y, float z) {
		rotate({ x, y, z });
	}

	mat4x4 Camera::getView() {
		mat4x4 view;
		view.setIdentity();

		view = view * mat4x4::makeTranslation(-location.x, -location.y, -location.z) * mat4x4::makeRotation(rotation.x, rotation.y, rotation.z);

		return view;
	}

	vec3f Camera::getLocation() { 
		return { location.x, location.y, location.z }; 
	}

	void Camera::setLocation(vec3f loc) {
		location = loc;
	}

	void Camera::addLocation(vec3f loc) {
		location += loc;
	}

	void Camera::addLocation(float x, float y, float z) {
		addLocation({ x, y, z });
	}
}