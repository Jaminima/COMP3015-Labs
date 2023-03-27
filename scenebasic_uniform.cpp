#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;

#include <iostream>
using std::cerr;
using std::endl;

#include "helper/glutils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <thread>
#include <chrono>

#include "Asset.h"
#include "utils.h"
#include <glfw3.h>

using glm::vec3;

SceneBasic_Uniform::SceneBasic_Uniform() : angle(0.0f) {}

Asset fox("fox.obj", vec3(0, -1, -10));
Asset submarine("submarine.obj", vec3(5, -1, 50));
Asset torus("torus2.obj", vec3(0, -1, -2));
Asset torus_nodump("torus2.obj", vec3(0, -1, -2));
Asset cube("cube2.obj", vec3(-2, 0, -2));

void SceneBasic_Uniform::initScene()
{
	compile();

	Lighting l1 = Lighting();

	l1.ambient = vec4(0.3, 0.3, 0.3, 1);
	l1.diffuse = vec4(0.4, 0.4, 0.4, 1);
	l1.specular = vec4(1, 1, 1, 1);
	l1.Position = vec3(5, 10, -3);

	sceneObjects.AddLight(l1);

	Lighting l2 = Lighting();

	l2.ambient = vec4(0.3, 0.3, 0.3, 1);
	l2.diffuse = vec4(0.4, 0, 0, 1);
	l2.specular = vec4(0, 1, 1, 1);
	l2.Position = vec3(5, -10, -3);

	sceneObjects.AddLight(l2);

	sceneObjects.UpdateAllLightViews(&sceneObjects.cam);

	cube.Load();
	cube.assetData->mat.ambient = vec4(0.1);
	cube.assetData->mat.shininess = 10;
	cube.assetData->mat.AddTexture(prog.getHandle(), "stone.png");
	cube.assetData->mat.AddTexture(prog.getHandle(), "dirt.png");
	cube.Build();
	cube.Dump();

	torus.Load();
	torus.assetData->mat.ambient = vec4(0.1);
	torus.assetData->mat.shininess = 10;
	torus.Build();
	torus.Dump();

	/*torus_nodump.Load(true);
	torus_nodump.assetData->mat.ambient = vec4(0.1);
	torus_nodump.assetData->mat.shininess = 10;
	torus_nodump.Build();

	bool eq = torus.AssetEqual(&torus_nodump);*/

	fox.Load();
	fox.assetData->mat.ambient = vec4(0.1);
	fox.assetData->mat.shininess = 10;
	fox.assetData->scale = vec3(0.1);
	fox.assetData->mat.AddTexture(prog.getHandle(), "fox.png");
	fox.assetData->mat.AddTexture(prog.getHandle(), "dirt.png");
	fox.Build();
	fox.Dump();

	submarine.Load();
	submarine.assetData->mat.ambient = vec4(0.1);
	submarine.assetData->mat.shininess = 10;
	submarine.assetData->scale = vec3(0.1);
	submarine.Build();
	submarine.Dump();

	std::cout << std::endl;

	prog.printActiveUniforms();

	cube.Draw();
	torus.Draw();
	fox.Draw();
	submarine.Draw();

	sceneObjects.cam.updateMatrix();
}

void SceneBasic_Uniform::compile()
{
	try {
		//prog.compileShader("shader/phong/phong.vert");
		//prog.compileShader("shader/phong/blinn.vert");
		//prog.compileShader("shader/phong/blinnMulti.vert");
		prog.compileShader("shader/configurable/configurable.vert");

		//prog.compileShader("shader/phong/phong.frag");
		//prog.compileShader("shader/toon.frag");
		prog.compileShader("shader/configurable/configurable.frag");
		prog.link();
		prog.use();
	}
	catch (GLSLProgramException& e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update(float dt)
{
	processKeys(dt);

	vec3* cubeRot = &cube.assetData->rotation;

	cubeRot->z += 0.8f * dt;
	cubeRot->x += 0.4f * dt;

	boundAngles(cubeRot);

	/*vec3* foxRot = &fox.assetData->rotation;

	foxRot->z += 0.8f * dt;
	foxRot->y += 0.4f * dt;

	boundAngles(cubeRot);*/

	/*vec3* stoneRot = &stone.assetData->rotation;

	stoneRot->x += 0.8f * dt;
	stoneRot->y += 0.8f * dt;

	boundAngles(stoneRot);*/
}

void SceneBasic_Uniform::render()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	GLuint programHandle = prog.getHandle();

	sceneObjects.SetShaderConfig(programHandle);

	sceneObjects.SetAllLightUniforms(programHandle);

	cube.Render(programHandle, &sceneObjects);
	torus.Render(programHandle, &sceneObjects);
	fox.Render(programHandle, &sceneObjects);
	submarine.Render(programHandle, &sceneObjects);
}

void SceneBasic_Uniform::resize(int w, int h)
{
	width = w;
	height = h;
	glViewport(0, 0, w, h);
	sceneObjects.cam.aspect = w / h;
	sceneObjects.cam.updateMatrix();
}

void SceneBasic_Uniform::keyActve(int key, int mods, float dt)
{
	const float moveStep = 20.0f;

	vec3 offset = vec3();

	if (mods == GLFW_MOD_ALT) {
		switch (key)
		{
		case 'G':
			sceneObjects.shaderConf.toggle_global = !sceneObjects.shaderConf.toggle_global;
			break;
		case 'A':
			sceneObjects.shaderConf.toggle_ambient = !sceneObjects.shaderConf.toggle_ambient;
			break;
		case 'D':
			sceneObjects.shaderConf.toggle_diffuse = !sceneObjects.shaderConf.toggle_diffuse;
			break;
		case 'S':
			sceneObjects.shaderConf.toggle_specular = !sceneObjects.shaderConf.toggle_specular;
			break;
		}
	}

	else {
		switch (key) {
		case 'W':
			offset.z += moveStep * dt;
			break;

		case 'S':
			offset.z -= moveStep * dt;
			break;

		case 'A':
			offset.x += moveStep * dt;
			break;

		case 'D':
			offset.x -= moveStep * dt;
			break;

		case 'Z':
			offset.y -= moveStep * dt;
			break;

		case 'X':
			offset.y += moveStep * dt;
			break;
		case 'T':
			if (mods == GLFW_MOD_SHIFT) {
				sceneObjects.shaderConf.toonBands++;
			}
			else if (mods == GLFW_MOD_CONTROL) {
				sceneObjects.shaderConf.toonBands--;
			}
			else {
				sceneObjects.shaderConf.enableToon = !sceneObjects.shaderConf.enableToon;
			}
			break;
		}
	}

	sceneObjects.cam.updatePosition(offset);

	sceneObjects.cam.updateMatrix();
	sceneObjects.UpdateAllLightViews(&sceneObjects.cam);
}

void SceneBasic_Uniform::mouseMove(int x, int y)
{
	const float rotStep = 0.01f;

	int dx = cMouseX - x;
	int dy = cMouseY - y;

	sceneObjects.cam.rotation += vec3(dy * rotStep, dx * rotStep, 0);

	sceneObjects.cam.updateMatrix();

	cMouseX = x;
	cMouseY = y;
}