#ifndef __Asset
#define __Asset

#include <iostream>
#include <vector>
#include <string>
#include "helper/glutils.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp> // GLM: translate, rotate
#include <glm/ext/matrix_clip_space.hpp> // GLM: perspective and ortho
#include <glm/gtc/type_ptr.hpp> // GLM: access to the value_ptr
#include <glad/glad.h>

#include "SceneObjects.h"
#include "Camera.h"
#include "helper/glslprogram.h"

#define bufferCount 3

using namespace std;
using namespace glm;

class Material {
public:
	vec4 ambient;
	float shininess;

	int activeTextureLayers = 0;
	struct textureLayer {
		GLuint faceTexture;
	} textureLayers[10];

	void SetUniforms(GLuint programHandle);
};

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

	vec3 scale = vec3(1);

	Material mat;
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

	bool MeshEqual(Mesh* m);

	void Build(bool generateColours = false);

	void Draw();

	void Dump(ofstream* fileStr);

	void Render(GLuint programHandle, AssetData* assetData, SceneObjects* sceneObjects);
};

class Asset {
public:

	string srcFile;
	string* srcData = 0x0;
	bool loadedFromDump = false; 

	AssetData* assetData;

	vector<string> materialFiles;

	vector<Mesh> meshses;

	Asset(string srcFile);

	Asset(string srcFile, vec3 pos = vec3(), vec3 rot = vec3());

	bool AssetEqual(Asset* a);

	void Load(bool ignoreDump = false);

	bool TryLoadDump();

	void Dump();

	void ParseOBJ();

	void ParseOBJLine(string line);

	void ExecuteOBJOperation(string opCode, vector<string> operands);

	void AppendPointToMesh(string idx, Mesh* m);

	void Build(bool generateColours = false);

	void Draw();

	void Render(GLuint programHandle, SceneObjects* sceneObjects);

	void AddTexture(GLuint program, string file);
};

#endif