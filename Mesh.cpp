#include "Asset.h"
#include "helper/glutils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

void Mesh::Draw()
{
	glGenBuffers(bufferCount, this->buffers);

	glBindBuffer(GL_ARRAY_BUFFER, this->buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, this->data->vertexSet.size() * sizeof(vec3), &this->data->vertexSet[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, this->buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, this->data->colourSet.size() * sizeof(vec4), &this->data->colourSet[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &this->vaoBuffer);
	glBindVertexArray(this->vaoBuffer);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexBuffer(0, this->buffers[0], 0, sizeof(vec3));
	glBindVertexBuffer(1, this->buffers[1], 0, sizeof(vec4));

	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(0, 0);

	glVertexAttribFormat(1, 4, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(1, 1);

	glBindVertexArray(0);
}

Mesh::Mesh(string mesh_name)
{
	this->name = mesh_name;
	this->components = new MeshComponents();
}

void Mesh::Build(bool generateColours)
{
	if (this->components == 0x0) throw exception("Component Data Is Missing During OBJ Parsing!");

	this->data = new MeshData();

	int indexCount = this->components->indexSet.size();
	for (int i = 0; i < indexCount; i++) {
		ivec3 idx = this->components->indexSet[i];

		this->data->vertexSet.push_back(this->components->vertexSet[idx.x - 1]);
		this->data->texCooSet.push_back(this->components->texCooSet[idx.y - 1]);
		this->data->normalSet.push_back(this->components->normalSet[idx.z - 1]);

		if (generateColours) {
			this->data->colourSet.push_back(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}

	delete this->components;
	this->components = 0x0;
}

void Mesh::Render()
{
	glBindVertexArray(this->vaoBuffer);
	glDrawArrays(GL_TRIANGLES, 0, this->data->vertexSet.size());

	glBindVertexArray(0);
}
