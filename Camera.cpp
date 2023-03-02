#include "Camera.h"

#define DegreesToRadians (3.14159f/180)

Camera::Camera()
{
	this->updateMatrix();
}

void Camera::updateMatrix() {
	projectionMatrix = glm::perspective(fov, aspect, clipNear, clipFar);

	boundAngles(&rotation);

	/*viewMatrix = mat4(1.0f);

	viewMatrix = glm::rotate(viewMatrix, rotation[0], vec3(1.0f, 0.0f, 0.0f));
	viewMatrix = glm::rotate(viewMatrix, rotation[1], vec3(0.0f, 1.0f, 1.0f));
	viewMatrix = glm::rotate(viewMatrix, rotation[2], vec3(0.0f, 0.0f, 1.0f));

	viewMatrix = glm::translate(viewMatrix, position);*/

	vec3 radianRot = rotation * DegreesToRadians;

	lookingAt = vec3(cosf(radianRot.y)*cosf(radianRot.z), sinf(radianRot.z), sinf(radianRot.y) * cosf(radianRot.z));

	viewMatrix = glm::lookAt(position, position+lookingAt, vec3(0, 1, 0));
}
