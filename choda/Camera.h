#pragma once

#include <glm/glm.hpp>

namespace choda {

	class Camera
	{
	public:
		enum class Direction { Left, Right, Forward, Backward };
		Camera(glm::vec3 position, glm::vec3 worldUp);

	public:
		void move(Camera::Direction direction, float dt);
		void rotate(float pitch, float yaw, bool constrainPitch = true);

		// getter(s) & setter(s)
		float getSensitivity() const;
		glm::mat4 getViewMatrix() const;
		glm::vec3 getPosition() const;

		void setSensitivity(float sensitivity);
	private:
		glm::vec3 position;
		glm::vec3 front;
		glm::vec3 right;
		glm::vec3 up;
		glm::vec3 worldUp;
		float yaw, pitch;
		float sensitivity;
	};

}


