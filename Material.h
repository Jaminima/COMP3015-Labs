#ifndef __Mat
#define __Mat

#include <glm/glm.hpp>
#include "helper/glutils.h"
#include <string>

using namespace glm;
using namespace std;

class Material {
public:
	vec4 ambient;
	float shininess;

	int activeTextureLayers = 0;
	struct textureLayer {
		GLuint faceTexture;
	} textureLayers[10];

	void SetUniforms(GLuint programHandle);

	void AddTexture(GLuint program, string file);
};

#endif