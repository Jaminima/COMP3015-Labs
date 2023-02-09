#include "Asset.h"
#include "helper/glutils.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp> // GLM: translate, rotate
#include <glm/ext/matrix_clip_space.hpp> // GLM: perspective and ortho 
#include <glm/gtc/type_ptr.hpp> // GLM: access to the value_ptr

using namespace std;
using namespace glm;

void Mesh::Draw()
{
	glGenBuffers(bufferCount, this->buffers);

	glBindBuffer(GL_ARRAY_BUFFER, this->buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, this->data->vertexSet.size() * sizeof(vec3), this->data->vertexSet.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, this->buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, this->data->colourSet.size() * sizeof(vec4), this->data->colourSet.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, this->buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, this->data->normalSet.size() * sizeof(vec3), this->data->normalSet.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, this->buffers[3]);
	glBufferData(GL_ARRAY_BUFFER, this->data->texCooSet.size() * sizeof(vec2), this->data->texCooSet.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &this->vaoBuffer);
	glBindVertexArray(this->vaoBuffer);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexBuffer(0, this->buffers[0], 0, sizeof(vec3));
	glBindVertexBuffer(1, this->buffers[1], 0, sizeof(vec4));
	glBindVertexBuffer(2, this->buffers[2], 0, sizeof(vec3));
	glBindVertexBuffer(3, this->buffers[3], 0, sizeof(vec2));

	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(0, 0);

	glVertexAttribFormat(1, 4, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(1, 1);

	glVertexAttribFormat(2, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(2, 2);

	glVertexAttribFormat(3, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(2, 2);

	glBindVertexArray(0);
}

Mesh::Mesh(string mesh_name)
{
	this->name = mesh_name;
	this->components = new MeshComponents();
}

template<typename T>
inline void PushFaceIdx(int idx, std::vector<T>* data, std::vector<T>* components) {
	if (idx != 0) {
		if (idx == -1) idx = components->size();

		data->push_back(components->at(idx - 1));
	}
}

void Mesh::Build(bool generateColours)
{
	if (this->components == 0x0) throw exception("Component Data Is Missing During OBJ Parsing!");

	this->data = new MeshData();

	int indexCount = this->components->indexSet.size();
	int thirdIndexCount = indexCount / 3.0f;
	for (int i = indexCount - 1; i >= 0; i--) {
		ivec3 idx = this->components->indexSet[i];

		PushFaceIdx(idx.x, &this->data->vertexSet, &this->components->vertexSet);
		PushFaceIdx(idx.y, &this->data->texCooSet, &this->components->texCooSet);
		PushFaceIdx(idx.z, &this->data->normalSet, &this->components->normalSet);

		if (generateColours) {
			int cIdx = i / thirdIndexCount;
			float c = (float)(i - (thirdIndexCount*cIdx)) / thirdIndexCount;
			this->data->colourSet.push_back(vec4(1.0f)/*vec4(i % 3 == 0 ? 1 : 0, i % 3 == 1 ? 1 : 0, i % 3 == 2 ? 1 : 0, 1.0f)*//*vec4(cIdx ==0?c : 0.0f, cIdx == 1 ? c : 0.0f, cIdx == 2 ? c : 0.0f, 1.0f)*/);
		}
	}

	delete this->components;
	this->components = 0x0;
}

void Mesh::Render(GLSLProgram* prog, AssetData* assetData, Camera* camera)
{
	GLuint programHandle = prog->getHandle();

	glm::mat4 model(1.0f);
	model = glm::translate(model, vec3(assetData->position));

	model = glm::rotate(model, assetData->rotation[0], vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, assetData->rotation[1], vec3(0.0f, 1.0f, 1.0f));
	model = glm::rotate(model, assetData->rotation[2], vec3(0.0f, 0.0f, 1.0f));

	GLuint modelRef = glGetUniformLocation(programHandle, "ModelMatrix");
	glUniformMatrix4fv(modelRef, 1, GL_FALSE, glm::value_ptr(model));

	GLuint projRef = glGetUniformLocation(programHandle, "ProjectionMatrix");
	glUniformMatrix4fv(projRef, 1, GL_FALSE, glm::value_ptr(camera->projectionMatrix));

	glm::mat4 mv = camera->projectionMatrix * model;

	GLuint modelViewRef = glGetUniformLocation(programHandle, "ModelViewMatrix");
	glUniformMatrix4fv(modelViewRef, 1, GL_FALSE, glm::value_ptr(mv));

	GLuint normRef = glGetUniformLocation(programHandle, "NormalMatrix");
	glm::mat3 normalMatrix = mat3(glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2]));
	glUniformMatrix3fv(normRef, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	glBindVertexArray(this->vaoBuffer);
	glDrawArrays(GL_TRIANGLES, 0, this->data->vertexSet.size());

	glBindVertexArray(0);
}
