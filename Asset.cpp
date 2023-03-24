#include "Asset.h"
#include "utils.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "TextureLoad.h"
#include "SceneObjects.h"
#include "Light.h"
#include "helper/glslprogram.h"

using namespace glm;
using namespace std;

Asset::Asset(string srcFile)
{
	this->srcFile = srcFile;
	this->assetData = new AssetData();
}

void Asset::Load(bool ignoreDump)
{
	if (!ignoreDump && TryLoadDump()) return;

	ifstream file(this->srcFile);
	string str;
	if (file) {
		ostringstream ss;
		ss << file.rdbuf(); // reading data
		str = ss.str();
	}

	this->srcData = new string(str);

	if (this->srcFile.find(".obj") != string::npos) {
		ParseOBJ();
	}
}

void Asset::ParseOBJ()
{
	if (this->srcData == 0x0) throw exception("Source Data Is Missing During OBJ Parsing!");

	string line;
	int start = 0, end = -1;
	int dataLen = this->srcData->length();

	while (start < dataLen) {
		end = this->srcData->find("\n", start);

		if (end == string::npos) end = dataLen;

		line = this->srcData->substr(start, end - start);

		ParseOBJLine(line);

		start = end + 1;

		printf("Loading Asset From %s %i/%i BYTES\r", this->srcFile.c_str(), end, dataLen);
	}

	std::cout << std::endl;

	delete this->srcData;
	this->srcData = 0x0;
}

void Asset::ParseOBJLine(string line)
{
	int opCodeEnd = line.find(" ");
	string opCode = line.substr(0, opCodeEnd);

	if (opCode == "#") return;

	string operandString = line.substr(opCodeEnd + 1);

	vector<string> operands = split(operandString, " ");

	ExecuteOBJOperation(opCode, operands);
}

void Asset::ExecuteOBJOperation(string opCode, vector<string> operands)
{
	if (opCode == "o") {
		if (meshses.size() == 1 && meshses[0].name == "No Name") {
			meshses[0].name = operands[0];
		}
		else {
			Mesh m(operands[0]);

			this->meshses.push_back(m);
		}

		return;
	}

	else if (opCode == "mtllib") {
		this->materialFiles.push_back(operands[0]);

		return;
	}

	if (this->meshses.size() == 0) {
		printf("Unnamed Object Detected %s                            \n", this->srcFile.c_str());

		Mesh m("No Name");

		this->meshses.push_back(m);

		//throw exception("No Existing Meshes To Operate On");
	}

	Mesh* m = &this->meshses.back();

	if (opCode == "usemtl") m->material = operands[0];

	else if (opCode == "v") {
		vec3 vertex(stof(operands[0]), stof(operands[1]), stof(operands[2]));

		m->components->vertexSet.push_back(vertex);
	}

	else if (opCode == "vn") {
		vec3 normal(stof(operands[0]), stof(operands[1]), stof(operands[2]));

		m->components->normalSet.push_back(normal);
	}

	else if (opCode == "vt") {
		vec2 texCoor(stof(operands[0]), stof(operands[1]));

		m->components->texCooSet.push_back(texCoor);
	}

	else if (opCode == "f") {
		switch (operands.size())
		{
		case 3:
			AppendPointToMesh(operands[0], m);
			AppendPointToMesh(operands[1], m);
			AppendPointToMesh(operands[2], m);
			break;
		case 4:
			AppendPointToMesh(operands[0], m);
			AppendPointToMesh(operands[1], m);
			AppendPointToMesh(operands[2], m);

			AppendPointToMesh(operands[2], m);
			AppendPointToMesh(operands[3], m);
			AppendPointToMesh(operands[0], m);

			break;
		default:
			throw exception("Improper Face Count");
			break;
		}
	}
}

void Asset::AppendPointToMesh(string idx, Mesh* m)
{
	vector<string> faceData = split(idx, "/");

	ivec3 face;

	switch (faceData.size())
	{
	case 1:
		face = ivec3(
			(faceData[0].empty() ? 0 : stoi(faceData[0])),
			0,
			0
		);
		break;
	case 2:
		face = ivec3(
			(faceData[0].empty() ? 0 : stoi(faceData[0])),
			(faceData[1].empty() ? 0 : stoi(faceData[1])),
			0
		);
		break;
	case 3:
		face = ivec3(
			(faceData[0].empty() ? 0 : stoi(faceData[0])),
			(faceData[1].empty() ? 0 : stoi(faceData[1])),
			(faceData[2].empty() ? 0 : stoi(faceData[2]))
		);
		break;
	}

	m->components->indexSet.push_back(face);
}

void Asset::Build(bool generateColours)
{
	if (this->loadedFromDump) {
		return;
	}

	int meshCount = this->meshses.size();
	for (int i = 0; i < meshCount; i++) {
		this->meshses[i].Build(generateColours);
	}
}

void Asset::Draw()
{
	int meshCount = this->meshses.size();
	for (int i = 0; i < meshCount; i++) {
		this->meshses[i].Draw();
	}
}

Asset::Asset(string srcFile, vec3 pos, vec3 rot)
{
	this->srcFile = srcFile;
	this->assetData = new AssetData();
	this->assetData->position = pos;
	this->assetData->rotation = rot;
}

void Asset::Render(GLSLProgram* prog, SceneObjects* sceneObjects)
{
	GLuint programHandle = prog->getHandle();
	assetData->mat.SetUniforms(programHandle);
	sceneObjects->masterLight.SetUniforms(programHandle);

	int meshCount = this->meshses.size();
	for (int i = 0; i < meshCount; i++) {
		if (this->hasTexture) glBindTexture(GL_TEXTURE_2D, this->texture);
		this->meshses[i].Render(programHandle, this->assetData, sceneObjects);
	}
}

void Asset::AddTexture(GLuint program, string file)
{
	loadTexture(this->texture, file);
	glUniform1i(glGetUniformLocation(program, "texture"), 0);
	this->hasTexture = true;
}

inline void Material::SetUniforms(GLuint programHandle) {
	GLuint ref = glGetUniformLocation(programHandle, "Mat.ambient");
	glUniform4fv(ref, 1, glm::value_ptr(ambient));

	ref = glGetUniformLocation(programHandle, "Mat.shininess");
	glUniform1f(ref, shininess);
}