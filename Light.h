#ifndef __Light
#define __Light
#include "Camera.h"
#include <glm/glm.hpp>
#include "helper/glutils.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp> // GLM: translate, rotate
#include <glm/ext/matrix_clip_space.hpp> // GLM: perspective and ortho
#include <glm/gtc/type_ptr.hpp> // GLM: access to the value_ptr
using namespace glm;

class Lighting {
private:
	vec4 ViewPosition;

public:
	vec4 ambient;
	vec4 diffuse;
	vec3 Position;
	vec4 specular;

	Lighting(vec4 _ambient = vec4(0), vec4 _diffuse = vec4(0), vec3 _position = vec3(0), vec4 _specular = vec4(0)) {
		this->ambient = _ambient;
		this->diffuse = _diffuse;
		this->Position = _position;
		this->specular = _specular;

		this->ViewPosition = vec4(0);
	}

	void SetUniforms(GLuint programHandle, int lightId = 0) {
		string lightStr = "Light[" + to_string(lightId) + "].";

		GLuint ref = glGetUniformLocation(programHandle, (lightStr + "ambient").c_str());
		glUniform4fv(ref, 1, glm::value_ptr(ambient));

		ref = glGetUniformLocation(programHandle, (lightStr + "diffuse").c_str());
		glUniform4fv(ref, 1, glm::value_ptr(diffuse));

		ref = glGetUniformLocation(programHandle, (lightStr + "Position").c_str());
		glUniform3fv(ref, 1, glm::value_ptr(Position));

		ref = glGetUniformLocation(programHandle, (lightStr + "ViewPosition").c_str());
		glUniform4fv(ref, 1, glm::value_ptr(ViewPosition));

		ref = glGetUniformLocation(programHandle, (lightStr + "specular").c_str());
		glUniform4fv(ref, 1, glm::value_ptr(specular));
	}

	void UpdateView(Camera* cam) {
		ViewPosition = vec4(cam->viewMatrix * vec4(Position, 1));
	}
};

#endif // !__Light
