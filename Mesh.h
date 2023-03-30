#ifndef __Mesh
#define __Mesh

#include "Material.h"
#include "SceneObjects.h"
#include "Asset.h"

#include <vector>

#define bufferCount 3

class AssetData;

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

	Mesh* parentMesh = 0x0;
	string name;
	string material;

	Mesh(string mesh_name);

	bool MeshEqual(Mesh* m);

	void Build(bool generateColours = false);

	void Draw();

	void Dump(ofstream* fileStr);

	void Render(GLuint programHandle, AssetData* assetData, SceneObjects* sceneObjects);
};

#endif