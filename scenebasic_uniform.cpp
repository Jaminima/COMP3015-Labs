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

#include "Asset.h"
#include "utils.h"

using glm::vec3;

SceneBasic_Uniform::SceneBasic_Uniform() : angle(0.0f) {}

Asset fox("./Assets/fox.obj", vec3(0, -1, 10));
Asset train("./Assets/steam-train.obj", vec3(10, -1, 0));
Asset stone("./Assets/torus2.obj", vec3(0, -1, -2));
Asset cube("./Assets/cube.obj", vec3(10, 0, -5));

void SceneBasic_Uniform::initScene()
{
	compile();

	sceneObjects.masterLight.ambient = vec4(0.1, 0.1, 0.1, 1);
	sceneObjects.masterLight.diffuse = vec4(0.4, 0.4, 0.4, 1);
	sceneObjects.masterLight.specular = vec4(1, 1, 1, 1);
	sceneObjects.masterLight.Position = vec3(5, -10, -3);

	sceneObjects.masterLight.UpdateView(&sceneObjects.cam);

	cube.Load();
	cube.assetData->mat.ambient = vec4(0.1);
	cube.assetData->mat.shininess = 10;
	cube.AddTexture(prog.getHandle(), "./Assets/cube.png");
	cube.Build();
	cube.Dump();

	stone.Load();
	stone.assetData->mat.ambient = vec4(0.1);
	stone.assetData->mat.shininess = 10;
	stone.Build();
	stone.Dump();

	/*fox.Load();
	fox.assetData->mat.ambient = vec4(0.1);
	fox.assetData->mat.shininess = 10;
	fox.Build();

	train.Load();
	train.assetData->mat.ambient = vec4(0.1);
	train.assetData->mat.shininess = 10;
	train.Build();*/

	std::cout << std::endl;

	prog.printActiveUniforms();

	cube.Draw();
	stone.Draw();

	sceneObjects.cam.updateMatrix();
}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
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

	cube.Render(&prog, &sceneObjects);
	stone.Render(&prog, &sceneObjects);
	fox.Render(&prog, &sceneObjects);
	train.Render(&prog, &sceneObjects);
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
	}

	sceneObjects.cam.updatePosition(offset);

	sceneObjects.cam.updateMatrix();
	sceneObjects.masterLight.UpdateView(&sceneObjects.cam);
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