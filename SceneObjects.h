#ifndef __SceneObjs
#define __SceneObjs
#include "Camera.h"
#include "Light.h"

class SceneObjects {
public:
	Camera cam;

	Lighting lights[10];
	int activeLights = 0;

	struct ShaderConfig
	{
	public:
		bool enableToon = false;
		int toonBands = 10;

	} shaderConf;

	void AddLight(Lighting l) {
		lights[activeLights] = l;
		activeLights++;
	}

	void SetShaderConfig(GLuint programHandle) {
		GLuint ref = glGetUniformLocation(programHandle, "enableToon");
		glUniform1i(ref, shaderConf.enableToon);

		ref = glGetUniformLocation(programHandle, "toonBands");
		glUniform1i(ref, shaderConf.toonBands);
	}

	void SetAllLightUniforms(GLuint programHandle) {
		GLuint ref = glGetUniformLocation(programHandle, "lights");
		glUniform1i(ref, activeLights);

		for (int i = 0; i < activeLights; i++) {
			lights[i].SetUniforms(programHandle,i);
		}
	}

	void UpdateAllLightViews(Camera* cam) {
		for (int i = 0; i < activeLights; i++) {
			lights[i].UpdateView(cam);
		}
	}
};

#endif // !__SceneObjs
