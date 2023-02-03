#include "Asset.h"

Mesh::Mesh(string mesh_name)
{
	this->name = mesh_name;
	this->components = new MeshComponents();
}

void Mesh::Build()
{
	if (this->components == 0x0) throw exception("Component Data Is Missing During OBJ Parsing!");

	this->data = new MeshData();

	int indexCount = this->components->indexSet.size();
	for (int i = 0; i < indexCount; i++) {
		ivec3 idx = this->components->indexSet[i];

		this->data->vertexSet.push_back(this->components->vertexSet[idx.x - 1]);
		this->data->texCooSet.push_back(this->components->texCooSet[idx.y - 1]);
		this->data->normalSet.push_back(this->components->normalSet[idx.z - 1]);
	}

	delete this->components;
	this->components = 0x0;
}
