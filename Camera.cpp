#include "Camera.h"

#define PI 3.14159f
#define DegreesToRadians (PI/180)

Camera::Camera()
{
	projectionMatrix = glm::perspective(fov, aspect, clipNear, clipFar);

	this->updateMatrix();
}

void Camera::updateMatrix() {

	boundAngles(&rotation);

	/*viewMatrix = mat4(1.0f);

	viewMatrix = glm::rotate(viewMatrix, rotation[0], vec3(1.0f, 0.0f, 0.0f));
	viewMatrix = glm::rotate(viewMatrix, rotation[1], vec3(0.0f, 1.0f, 1.0f));
	viewMatrix = glm::rotate(viewMatrix, rotation[2], vec3(0.0f, 0.0f, 1.0f));

	viewMatrix = glm::translate(viewMatrix, position);*/

	vec3 radianRot = rotation * DegreesToRadians;

	float cosx = cosf(radianRot.x);
	float cosy = cosf(radianRot.y);
	float cosz = cosf(radianRot.z);

	float sinx = sinf(radianRot.x);
	float siny = sinf(radianRot.y);
	float sinz = sinf(radianRot.z);

	vec3 dir = vec3(0, -PI, 0);

	dir = vec3(
		dir.x,
		(cosx * dir.y) + (sinx * dir.z),
		(cosx * dir.z) - (sinx * dir.y));

	dir = vec3(
		(cosy * dir.x) + (siny * dir.z),
		dir.y,
		(cosy * dir.z) - (siny * dir.x));

	dir = vec3(
		(cosz * dir.x) + (sinz * dir.y),
		(cosz * dir.y) - (sinz * dir.x),
		dir.z);

	lookingAt = dir;


	//lookingAt = vec3(cosf(radianRot.y)*cosf(radianRot.z), sinf(radianRot.z), sinf(radianRot.y) * cosf(radianRot.z));

	viewMatrix = glm::lookAt(position, position+lookingAt, vec3(0, 1, 0));
}
