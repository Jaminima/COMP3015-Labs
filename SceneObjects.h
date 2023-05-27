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
		bool enableEdge = false;
		bool enableBlur = false;
		bool isPostProcessing = false;
		int toonBands = 10;

		bool toggle_global = 1, toggle_ambient = 1, toggle_diffuse = 1, toggle_specular = 1, toggle_blur = 1, toggle_edge = 1;
	} shaderConf;

	void AddLight(Lighting l) {
		lights[activeLights] = l;
		activeLights++;
	}

	void SetShaderConfig(GLuint programHandle) {
		GLuint ref = glGetUniformLocation(programHandle, "enableToon");
		glUniform1i(ref, shaderConf.enableToon);

		ref = glGetUniformLocation(programHandle, "enableEdge");
		glUniform1i(ref, shaderConf.enableEdge);


		ref = glGetUniformLocation(programHandle, "enableBlur");
		glUniform1i(ref, shaderConf.enableBlur);

		ref = glGetUniformLocation(programHandle, "isPostProcessing");
		glUniform1i(ref, shaderConf.isPostProcessing);

		ref = glGetUniformLocation(programHandle, "toonBands");
		glUniform1i(ref, shaderConf.toonBands);

		ref = glGetUniformLocation(programHandle, "toggle_global");
		glUniform1i(ref, shaderConf.toggle_global);

		ref = glGetUniformLocation(programHandle, "toggle_ambient");
		glUniform1i(ref, shaderConf.toggle_ambient);

		ref = glGetUniformLocation(programHandle, "toggle_diffuse");
		glUniform1i(ref, shaderConf.toggle_diffuse);

		ref = glGetUniformLocation(programHandle, "toggle_specular");
		glUniform1i(ref, shaderConf.toggle_specular);
	}

	void SetAllLightUniforms(GLuint programHandle) {
		GLuint ref = glGetUniformLocation(programHandle, "lights");
		glUniform1i(ref, activeLights);

		for (int i = 0; i < activeLights; i++) {
			lights[i].SetUniforms(programHandle, i);
		}
	}

	void UpdateAllLightViews(Camera* cam) {
		for (int i = 0; i < activeLights; i++) {
			lights[i].UpdateView(cam);
		}
	}
};

#endif // !__SceneObjs
