class Camera;

#ifndef __Camera
#define __Camera

#include "Asset.h"
#include <glm/glm.hpp>

#include <glm/ext/matrix_transform.hpp> // GLM: translate, rotate
#include <glm/ext/matrix_clip_space.hpp> // GLM: perspective and ortho 

using namespace glm;

class Camera {
public:
	glm::mat4 projectionMatrix;

	vec3 position;
	vec3 rotation;

	float fov = 90.0f;
	float aspect = 1.0f;

	float clipNear = 0.1f;
	float clipFar = 1000.0f;

	void updateMatrix() {
		projectionMatrix = glm::perspective(fov, aspect, clipNear, clipFar);
	
		projectionMatrix = glm::rotate(projectionMatrix, rotation[0], vec3(1.0f, 0.0f, 0.0f));
		projectionMatrix = glm::rotate(projectionMatrix, rotation[1], vec3(0.0f, 1.0f, 1.0f));
		projectionMatrix = glm::rotate(projectionMatrix, rotation[2], vec3(0.0f, 0.0f, 1.0f));

		projectionMatrix = glm::translate(projectionMatrix, position);
	}
};

#endif