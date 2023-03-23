#ifndef __Camera
#define __Camera

#include "utils.h"
#include <glm/glm.hpp>

#include <glm/ext/matrix_transform.hpp> // GLM: translate, rotate
#include <glm/ext/matrix_clip_space.hpp> // GLM: perspective and ortho 

using namespace glm;

class Camera {
public:
	Camera();

	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

	vec3 position;
	vec3 rotation = vec3(0,-90,0);
	vec3 lookingAt;

	float cosx;
	float cosy;
	float cosz;

	float sinx;
	float siny;
	float sinz;

	float fov = 90.0f;
	float aspect = 1.0f;

	float clipNear = 0.1f;
	float clipFar = 1000.0f;

	void updateMatrix();

	void updatePosition(vec3 offset);

	vec3 rotateDirection(vec3 dir);
};

#endif