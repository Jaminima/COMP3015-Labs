#include "Material.h"

#include <glm/gtc/type_ptr.hpp> // GLM: access to the value_ptr
#include "TextureLoad.h"

void Material::SetUniforms(GLuint programHandle) {
	GLuint ref = glGetUniformLocation(programHandle, "Mat.ambient");
	glUniform4fv(ref, 1, glm::value_ptr(ambient));

	ref = glGetUniformLocation(programHandle, "Mat.shininess");
	glUniform1f(ref, shininess);

	ref = glGetUniformLocation(programHandle, "activeTextureLayers");
	glUniform1i(ref, activeTextureLayers);

	for (int i = 0; i < activeTextureLayers; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textureLayers[i].faceTexture);
		glUniform1i(glGetUniformLocation(programHandle, ("textureLayers[" + to_string(i) + "].faceTexture").c_str()), i);
	}
}

void Material::AddTexture(GLuint program, string file)
{
	loadTexture(textureLayers[activeTextureLayers].faceTexture, "./assets/textures/" + file);
	activeTextureLayers++;
}
