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
#include "helper/glslprogram.h"

#include "Mesh.h"

using namespace std;
using namespace glm;

class AssetData {
public:
	vec3 position;
	vec3 rotation;

	vec3 scale = vec3(1);

	Material mat;
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
};

#endif