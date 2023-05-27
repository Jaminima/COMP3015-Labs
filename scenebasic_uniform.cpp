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

Asset fox("fox.obj", vec3(0, -1, -5));
Asset submarine("submarine.obj", vec3(5, -1, 50));
Asset torus("torus2.obj", vec3(0, -0.6, -2), vec3(40, 45, 0));
Asset flatplane("flatplane.obj", vec3(0, -1, 0));
Asset cube("cube2.obj", vec3(-2, 0, -2));
Asset fullScreenQuad("fullScreenQuad.obj", vec3());

void SceneBasic_Uniform::initScene()
{
	compile();
	initFBO();

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

	//Full Screen 
	fullScreenQuad.FullInit();
	Material fsqMat("fsqMat", vec4(0), 0);
	fsqMat.AddTexture(prog.getHandle(), "blank.png");

	fullScreenQuad.materials.push_back(fsqMat);
	fullScreenQuad.SetDefaultMat(&fullScreenQuad.materials.back());

	//Cube
	cube.FullInit();
	Material cubeMat("CubeMat", vec4(0.1), 10);
	cubeMat.AddTexture(prog.getHandle(), "stone.png");
	cubeMat.AddTexture(prog.getHandle(), "dirt.png");

	cube.materials.push_back(cubeMat);
	cube.SetDefaultMat(&cube.materials.back());


	//Torus
	torus.FullInit();
	Material torusMat("TorusMat", vec4(0.1), 10);

	torus.materials.push_back(torusMat);
	torus.SetDefaultMat(&torus.materials.back());


	//Flat Plane
	flatplane.FullInit();
	flatplane.assetData->scale = vec3(10);

	Material flatplaneMat("FlatPlaneMat", vec4(0.1), 10);
	flatplaneMat.AddTexture(prog.getHandle(), "floor.png");

	flatplane.materials.push_back(flatplaneMat);
	flatplane.SetDefaultMat(&flatplane.materials.back());


	//Fox
	fox.FullInit();
	fox.assetData->scale = vec3(0.05);

	Material foxMat("FoxMat", vec4(0.1), 10);
	foxMat.AddTexture(prog.getHandle(), "fox.png");
	foxMat.AddTexture(prog.getHandle(), "dirt.png");

	fox.materials.push_back(foxMat);
	fox.SetDefaultMat(&fox.materials.back());


	//Submarinee
	submarine.FullInit();
	submarine.assetData->scale = vec3(0.1);

	Material submarineMat("SubMat", vec4(0.1), 1);

	submarine.materials.push_back(submarineMat);
	submarine.SetDefaultMat(&submarine.materials.back());


	//Wrap Up
	std::cout << std::endl;

	prog.printActiveUniforms();

	sceneObjects.cam.updateMatrix();
}

void SceneBasic_Uniform::initFBO()
{
	//http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
	//https://gist.github.com/Hebali/6ebfc66106459aacee6a9fac029d0115
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glGenTextures(1, &renderedTexture);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

		prog.compileShader("shader/advanced/edge.frag");

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

	vec3* torRot = &torus.assetData->rotation;

	//torRot->x += 0.8f * dt;
	torRot->y += 1.8f * dt;

	boundAngles(torRot);
}

void SceneBasic_Uniform::primaryRender(GLuint programHandle) {
	sceneObjects.shaderConf.enableEdge = false;
	sceneObjects.shaderConf.isPostProcessing = false;
	sceneObjects.SetShaderConfig(programHandle);

	sceneObjects.SetAllLightUniforms(programHandle);

	cube.Render(programHandle, &sceneObjects);
	torus.Render(programHandle, &sceneObjects);
	fox.Render(programHandle, &sceneObjects);
	submarine.Render(programHandle, &sceneObjects);
	flatplane.Render(programHandle, &sceneObjects);
}

void SceneBasic_Uniform::postProcessing(GLuint programHandle, GLuint renderedTexture)
{
	sceneObjects.shaderConf.enableEdge = sceneObjects.shaderConf.toggle_edge;
	sceneObjects.shaderConf.enableBlur = sceneObjects.shaderConf.toggle_blur;
	sceneObjects.shaderConf.isPostProcessing = true;
	sceneObjects.SetShaderConfig(programHandle);

	fullScreenQuad.materials[0].textureLayers[0].faceTexture = renderedTexture;
	fullScreenQuad.Render(programHandle, &sceneObjects);
}



void SceneBasic_Uniform::render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	GLuint programHandle = prog.getHandle();

	primaryRender(programHandle);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	postProcessing(programHandle, renderedTexture);
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
		case 'E':
			sceneObjects.shaderConf.toggle_edge = !sceneObjects.shaderConf.toggle_edge;
			break;
		case 'B':
			sceneObjects.shaderConf.toggle_blur = !sceneObjects.shaderConf.toggle_blur;
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