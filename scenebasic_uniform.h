#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"

class SceneBasic_Uniform : public Scene
{
private:
	GLuint vaoHandle;
	GLSLProgram prog;
	GLSLProgram *advprog;
	float angle;

	void compile();

public:
	SceneBasic_Uniform();

	void initScene();
	void update(float t);
	void render();
	void resize(int, int);

	// Inherited via Scene
	virtual void keyActve(int key, int mods, float dt) override;
	virtual void mouseMove(int x, int y) override;
};

#endif // SCENEBASIC_UNIFORM_H
