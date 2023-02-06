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

Asset stone("./Assets/stone.obj", vec3(0, 0, -30));
Asset cube("./Assets/cube.obj", vec3(20,0,-20));

void SceneBasic_Uniform::initScene()
{
    compile();

    cube.Load();
    cube.Build(true);

    stone.Load();
    stone.Build(true);

    std::cout << std::endl;

    prog.printActiveUniforms();

    cube.Draw();
    stone.Draw();

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

void SceneBasic_Uniform::update(float dt)
{
    processKeys(dt);

    vec3* cubeRot = &cube.assetData->rotation;

    cubeRot->z += 0.8f * dt;
    cubeRot->x += 0.4f * dt;

    boundAngles(cubeRot);

    vec3* stoneRot = &stone.assetData->rotation;

    stoneRot->y += 0.8f * dt;

    boundAngles(stoneRot);
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    cube.Render(&prog, &cam);
    stone.Render(&prog, &cam);
}

void SceneBasic_Uniform::resize(int w, int h)
{
    width = w;
    height = h;
    glViewport(0,0,w,h);
}

void SceneBasic_Uniform::keyActve(int key, int mods, float dt)
{
    const float moveStep = 20.0f;

    switch (key) {
        case 'W':
            cam.position.z += moveStep * dt;
            break;

        case 'S':
            cam.position.z -= moveStep * dt;
            break;

        case 'A':
            cam.position.x += moveStep * dt;
            break;

        case 'D':
            cam.position.x -= moveStep * dt;
            break;
    }
    cam.updateMatrix();
}
