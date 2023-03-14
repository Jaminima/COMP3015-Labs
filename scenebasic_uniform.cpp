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

Asset stone("./Assets/torus.obj", vec3(0, -1, -2));
Asset cube("./Assets/cube.obj", vec3(10,0,-5));

void SceneBasic_Uniform::initScene()
{
    compile();

    sceneObjects.masterLight.ambient = vec4(0,0,0,1);
    sceneObjects.masterLight.diffuse = vec4(0,0,0,1);
    sceneObjects.masterLight.specular = vec4(1,1,1,1);
    sceneObjects.masterLight.Position = vec3(0, 2, -3);

    cube.Load();
    cube.assetData->mat.ambient = vec4(0.1);
    cube.assetData->mat.shininess = 100;
    cube.AddTexture(prog.getHandle(), "./Assets/cube.png");
    cube.Build(true);

    stone.Load();
    stone.assetData->mat.ambient = vec4(0.1);
    stone.assetData->mat.shininess = 100;
    stone.Build(true);

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
}

void SceneBasic_Uniform::resize(int w, int h)
{
    width = w;
    height = h;
    glViewport(0,0,w,h);
    sceneObjects.cam.aspect = w / h;
    sceneObjects.cam.updateMatrix();
}

void SceneBasic_Uniform::keyActve(int key, int mods, float dt)
{
    const float moveStep = 20.0f;

    switch (key) {
        case 'W':
            sceneObjects.cam.position.z -= moveStep * dt;
            break;

        case 'S':
            sceneObjects.cam.position.z += moveStep * dt;
            break;

        case 'A':
            sceneObjects.cam.position.x -= moveStep * dt;
            break;

        case 'D':
            sceneObjects.cam.position.x += moveStep * dt;
            break;
    }

    sceneObjects.cam.updateMatrix();
    sceneObjects.masterLight.UpdateView(&sceneObjects.cam);
}
