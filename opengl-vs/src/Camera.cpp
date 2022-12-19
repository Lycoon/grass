#include "Camera.h"
#include <iostream>

Camera::Camera(): Camera(glm::vec3(0.0f, 1.5f, 3.0f))
{}

Camera::Camera(glm::vec3 position): 
	position_(position),
	direction_(0.0f, -0.3f, -1.0f),
	up_(0.0f, 1.0f, 0.0f),
	fov_(DEFAULT_FOV),
	sensitivity_(DEFAULT_SENSITIVITY),
	speed_(DEFAULT_SPEED),
	yaw_(-90.0f),
	pitch_(0.0f)
{}

glm::vec3 Camera::getPosition() const { return position_; }
glm::vec3 Camera::getDirection() const { return direction_; }
float Camera::getSpeed() const { return speed_; }

glm::mat4 Camera::getPerspective(float aspectRatio) const {
	return glm::perspective(
		glm::radians(fov_), 
		aspectRatio,
		0.1f,
		800.0f
	);
}

glm::mat4 Camera::getView() const {
	return glm::lookAt(
		position_,
		position_ + direction_,
		up_
	);
}

void Camera::move(Direction dir)
{
	switch (dir)
	{
	case FRONT:
		position_ += direction_ * speed_;
		break;
	case BACK:
		position_ -= direction_ * speed_;
		break;
	case RIGHT:
		position_ += glm::normalize(glm::cross(direction_, up_)) * speed_;
		break;
	case LEFT:
		position_ -= glm::normalize(glm::cross(direction_, up_)) * speed_;
		break;
	case UP:
		position_ += up_ * speed_;
		break;
	}
}

void Camera::updateFOV(float scrollY)
{
	fov_ = glm::clamp(fov_ - scrollY, 10.0f, 80.0f);
}

void Camera::updateDirection(float yaw, float pitch)
{
	yaw_ += yaw * sensitivity_;
	pitch_ += pitch * sensitivity_;
	pitch_ = glm::clamp(pitch_, -89.0f, 89.0f);

	direction_.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	direction_.y = sin(glm::radians(pitch_));
	direction_.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	direction_ = glm::normalize(direction_);
}

void Camera::updateSpeed(float deltaTime) {
	speed_ = 3.0f * deltaTime;
}