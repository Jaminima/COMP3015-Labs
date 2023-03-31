#ifndef __Mesh
#define __Mesh

#include "Material.h"
#include "SceneObjects.h"
#include "Asset.h"

#include <vector>

#define bufferCount 3

class AssetData;
class SubMesh;

class MeshData {
public:
	vector<vec4> colourSet;

	vector<vec3> vertexSet;
	vector<vec3> normalSet;

	vector<vec2> texCooSet;
};

class MeshComponents : public MeshData {
public:
	vector<ivec3> indexSet;
};

class Mesh {
private:

public:
	GLuint buffers[bufferCount];
	GLuint vaoBuffer;

	MeshComponents* components;
	MeshData* data = 0x0;

	SubMesh* subMesh = 0x0;
	string name;
	Material* mat;

	Mesh(string mesh_name, Material* mat = 0x0);

	bool MeshEqual(Mesh* m);

	void Build(Mesh* parent);

	void Draw();

	void Dump(ofstream* fileStr, Mesh* parent);

	void Render(GLuint programHandle, AssetData* assetData, SceneObjects* sceneObjects);
};


class SubMesh {
public:
	Mesh* mesh;
	SubMesh* next;

	SubMesh(Mesh* m, SubMesh* s) {
		mesh = m;
		next = s;
	}

	void Draw()
	{
		mesh->Draw();
		if (next != 0x0) next->Draw();
	}

	void Build(Mesh* parent)
	{
		mesh->Build(parent);
		if (next != 0x0) next->Build(parent);
	}

	void Dump(ofstream* fileStr, Mesh* parent) {
		mesh->Dump(fileStr, parent);
		if (next != 0x0) next->Dump(fileStr, parent);
	}

	void Render(GLuint programHandle, AssetData* assetData, SceneObjects* sceneObjects) {
		mesh->Render(programHandle, assetData, sceneObjects);
		if (next != 0x0) next->Render(programHandle, assetData, sceneObjects);
	}
};

#endif