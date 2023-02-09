#include "Camera.h"

Camera::Camera()
{
	this->updateMatrix();
}

void Camera::updateMatrix() {
	projectionMatrix = glm::perspective(fov, aspect, clipNear, clipFar);

	boundAngles(&rotation);

	viewMatrix = mat4(1.0f);

	viewMatrix = glm::rotate(viewMatrix, rotation[0], vec3(1.0f, 0.0f, 0.0f));
	viewMatrix = glm::rotate(viewMatrix, rotation[1], vec3(0.0f, 1.0f, 1.0f));
	viewMatrix = glm::rotate(viewMatrix, rotation[2], vec3(0.0f, 0.0f, 1.0f));

	viewMatrix = glm::translate(viewMatrix, position);
}
