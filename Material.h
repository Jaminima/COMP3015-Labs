#ifndef __Mat
#define __Mat

#include <glm/glm.hpp>
#include "helper/glutils.h"
#include <string>

using namespace glm;
using namespace std;

class Material {
public:
	string name;

	vec4 ambient;
	float shininess;
	bool canDisintergrate = false;

	int activeTextureLayers = 0;
	struct textureLayer {
		GLuint faceTexture;
	} textureLayers[10];

	Material(string name, vec4 ambient = vec4(0), float shininess = 1);

	void SetUniforms(GLuint programHandle);

	void AddTexture(GLuint program, string file);
};

#endif