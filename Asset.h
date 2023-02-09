#ifndef __Asset
#define __Asset

#include <iostream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "helper/glslprogram.h"

#include "Camera.h"

#define bufferCount 4

using namespace std;
using namespace glm;

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

class AssetData {
public:
	vec3 position;
	vec3 rotation;
};

class Mesh {
private:

public:
	GLuint buffers[bufferCount];
	GLuint vaoBuffer;

	MeshComponents* components;
	MeshData* data = 0x0;

	string name;
	string material;

	Mesh(string mesh_name);

	void Build(bool generateColours = false);

	void Draw();

	void Render(GLSLProgram* prog, AssetData* assetData, Camera* camera);
};

class Asset {
public:

	string srcFile;
	string* srcData = 0x0;

	bool hasTexture = false;
	GLuint texture;

	AssetData* assetData;

	vector<string> materialFiles;

	vector<Mesh> meshses;

	Asset(string srcFile);

	Asset(string srcFile, vec3 pos = vec3(), vec3 rot = vec3());

	void Load();

	void ParseOBJ();

	void ParseOBJLine(string line);

	void ExecuteOBJOperation(string opCode, vector<string> operands);

	void AppendPointToMesh(string idx, Mesh* m);

	void Build(bool generateColours = false);

	void Draw();

	void Render(GLSLProgram* prog, Camera* camera);

	void AddTexture(GLuint program, string file);
};

#endif