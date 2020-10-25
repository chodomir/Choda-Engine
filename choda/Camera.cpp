#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace choda;

Camera::Camera(glm::vec3 position, glm::vec3 worldUp) : position(position), worldUp(worldUp),
front(0.0f, 0.0f, -1.0f), right(glm::cross(front, worldUp)), up(glm::cross(right, front)),
yaw(-90.0f), pitch(0.0f), sensitivity(0.05f)
{}

void Camera::move(Direction direction, float dt)
{
	float speed = 3.0f * dt;
	if (direction == Direction::Forward) {
		position += speed * front;
	} if (direction == Direction::Backward) {
		position -= speed * front;
	} if (direction == Direction::Left) {
		position -= speed * right;
	} if (direction == Direction::Right) {
		position += speed * right;
	}
}

void Camera::rotate(float pitch, float yaw, bool constrainPitch)
{
	this->pitch += pitch;
	this->yaw += yaw;

	if (constrainPitch) {
		if (this->pitch > 89.0f)
			this->pitch = 89.0f;
		if (this->pitch < -89.0f)
			this->pitch = -89.0f;
	}

	front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	front.y = sin(glm::radians(this->pitch));
	front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	// update camera basis vectors
	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

float Camera::getSensitivity() const
{
	return sensitivity;
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(position, position + front, up);
}

glm::vec3 choda::Camera::getPosition() const
{
	return position;
}

void choda::Camera::setSensitivity(float sensitivity)
{
	this->sensitivity = sensitivity;
}


