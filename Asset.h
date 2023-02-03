#ifndef __Asset
#define __Asset

#include <iostream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>

#define bufferCount 2

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

	void Render();
};

class Asset {
public:

	string srcFile;
	string* srcData = 0x0;

	vector<string> materialFiles;

	vector<Mesh> meshses;

	Asset(string srcFile);

	void Load();

	void ParseOBJ();

	void ParseOBJLine(string line);

	void ExecuteOBJOperation(string opCode, vector<string> operands);

	void Build(bool generateColours = false);

	void Draw();

	void Render();
};

#endif