#ifndef __SceneObjs
#define __SceneObjs
#include "Camera.h"
#include "Light.h"

class SceneObjects {
public:
	Camera cam;

	Lighting lights[10];
	int activeLights = 1;

	void SetAllLightUniforms(GLuint programHandle) {
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
