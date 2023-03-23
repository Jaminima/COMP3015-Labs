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
public:
	vec4 ambient;
	vec4 diffuse;
	vec3 Position;
	vec4 ViewPosition;
	vec4 specular;

	void SetUniforms(GLuint programHandle) {
		GLuint ref = glGetUniformLocation(programHandle, "Light.ambient");
		glUniform4fv(ref, 1, glm::value_ptr(ambient));

		ref = glGetUniformLocation(programHandle, "Light.diffuse");
		glUniform4fv(ref, 1, glm::value_ptr(diffuse));

		ref = glGetUniformLocation(programHandle, "Light.Position");
		glUniform3fv(ref, 1, glm::value_ptr(Position));

		ref = glGetUniformLocation(programHandle, "Light.ViewPosition");
		glUniform4fv(ref, 1, glm::value_ptr(ViewPosition));

		ref = glGetUniformLocation(programHandle, "Light.specular");
		glUniform4fv(ref, 1, glm::value_ptr(specular));
	}

	void UpdateView(Camera* cam) {
		ViewPosition = vec4(cam->viewMatrix * vec4(Position, 1));
	}
};

#endif // !__Light
