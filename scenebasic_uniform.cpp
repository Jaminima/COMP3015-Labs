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

glm::mat4 rotationMatrix;

void SceneBasic_Uniform::update( float t )
{
    //update your angle here
    angle = (angle>360 || angle<0) ? 0 : angle + 0.001f;
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    //create the rotation matrix here and update the uniform in the shader 

    rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, vec3(0.0f, 0.0f, 1.0f));

    GLuint programHandle = prog.getHandle();
    GLuint location = glGetUniformLocation(programHandle, "RotationMatrix");

    glUniformMatrix4fv(location, 1, GL_FALSE, &rotationMatrix[0][0]);


    /*glBindVertexArray(cube.meshses[0].vaoBuffer);
    glDrawArrays(GL_TRIANGLES, 0, cube.meshses[0].data->vertexSet.size());

    glBindVertexArray(0);*/

    cube.Render();
}

void SceneBasic_Uniform::resize(int w, int h)
{
    width = w;
    height = h;
    glViewport(0,0,w,h);
}
