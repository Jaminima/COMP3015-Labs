class Camera;

#ifndef __Camera
#define __Camera

#include "Asset.h"
#include <glm/glm.hpp>

using namespace glm;

class Camera {
public:
	vec3 position;
	vec3 rotation;

	float fov = 90.0f;
	float aspect = 1.0f;

	float clipNear = 0.1f;
	float clipFar = 1000.0f;
};

#endif