#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const float DEFAULT_FOV = 45.0f;
const float DEFAULT_SENSITIVITY = 0.1f;
const float DEFAULT_SPEED = 1.0f;

enum Direction {
	FRONT,
	BACK,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera {
public:
	Camera();
	Camera(glm::vec3 position);

	void updateFOV(float scrollY);
	void updateSpeed(float deltaTime);
	void move(Direction dir);
	void updateDirection(float yaw, float pitch);

	// getters
	glm::mat4 getPerspective(float aspectRatio) const;
	glm::mat4 getView() const;
	glm::vec3 getPosition() const;
	glm::vec3 getDirection() const;
	float getSpeed() const;

private:
	glm::vec3 position_;
	glm::vec3 direction_;
	const glm::vec3 up_;

	float fov_;
	float sensitivity_;
	float speed_;
	float yaw_;
	float pitch_;
};