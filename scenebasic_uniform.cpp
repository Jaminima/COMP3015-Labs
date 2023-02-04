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

Asset cube("./Assets/cube.obj");

void SceneBasic_Uniform::initScene()
{
    compile();

    cube.Load();
    cube.Build(true);

    std::cout << std::endl;

    prog.printActiveUniforms();

    cube.Draw();

    cam.updateMatrix();
}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::keyDown(int key) {
    printf("\rKey Pressed %c                         \n\r",key);
}

void SceneBasic_Uniform::update(float dt)
{
    vec3* cubeRot = &cube.assetData->rotation;

    //update your angle here

    cubeRot->z += 0.8f * dt;
    cubeRot->x += 0.4f * dt;

    boundAngles(cubeRot);

    cam.position.z -= 1.0f * dt;
    //cam.rotation.y += 0.001f;

    cam.updateMatrix();


    printf("Update Delta %f ms       \r", dt * 1000);
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    cube.Render(&prog, &cam);
}

void SceneBasic_Uniform::resize(int w, int h)
{
    width = w;
    height = h;
    glViewport(0,0,w,h);
}
