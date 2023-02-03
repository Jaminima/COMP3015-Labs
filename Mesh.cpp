#include "Asset.h"

Mesh::Mesh(string mesh_name)
{
	this->name = mesh_name;
	this->components = new MeshComponents();
}

void Mesh::Build()
{
}
