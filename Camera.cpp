#include "Camera.h"

Camera::Camera()
{
	this->updateMatrix();
}

void Camera::updateMatrix() {
	projectionMatrix = glm::perspective(fov, aspect, clipNear, clipFar);

	boundAngles(&rotation);

	projectionMatrix = glm::rotate(projectionMatrix, rotation[0], vec3(1.0f, 0.0f, 0.0f));
	projectionMatrix = glm::rotate(projectionMatrix, rotation[1], vec3(0.0f, 1.0f, 1.0f));
	projectionMatrix = glm::rotate(projectionMatrix, rotation[2], vec3(0.0f, 0.0f, 1.0f));

	projectionMatrix = glm::translate(projectionMatrix, position);
}
