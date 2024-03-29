#include "Material.h"

#include <glm/gtc/type_ptr.hpp> // GLM: access to the value_ptr
#include "TextureLoad.h"

Material::Material(string name, vec4 ambient, float shininess)
{
	this->name = name;
	this->ambient = ambient;
	this->shininess = shininess;
}

void Material::SetUniforms(GLuint programHandle) {
	GLuint ref = glGetUniformLocation(programHandle, "Mat.ambient");
	glUniform4fv(ref, 1, glm::value_ptr(ambient));

	ref = glGetUniformLocation(programHandle, "Mat.shininess");
	glUniform1f(ref, shininess);

	ref = glGetUniformLocation(programHandle, "activeTextureLayers");
	glUniform1i(ref, activeTextureLayers);

	ref = glGetUniformLocation(programHandle, "disintergrate");
	glUniform1i(ref, canDisintergrate);

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
