#include "Asset.h"
#include "strutils.h"

#include <iostream>
#include<fstream>
#include<sstream>
#include<string>

Asset::Asset(string srcFile)
{
	this->srcFile = srcFile;
    this->assetData = new AssetData();
}

void Asset::Load()
{
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
    }

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
        Mesh m(operands[0]);

        this->meshses.push_back(m);

        return;
    }

    else if (opCode == "mtllib") {
        this->materialFiles.push_back(operands[0]);

        return;
    }

    if (this->meshses.size() == 0) throw exception("No Existing Meshes To Operate On");

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
        for (int i = 0; i < operands.size(); i++) {
            vector<string> faceData = split(operands[i], "/");

            ivec3 face(
                (faceData[0].empty() ? -1 : stoi(faceData[0])),
                (faceData[1].empty() ? -1 : stoi(faceData[1])),
                (faceData[2].empty() ? -1 : stoi(faceData[2]))
            );

            m->components->indexSet.push_back(face);
        }
    }
}

void Asset::Build(bool generateColours)
{
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

void Asset::Render(GLSLProgram* prog)
{
    int meshCount = this->meshses.size();
    for (int i = 0; i < meshCount; i++) {
        this->meshses[i].Render(prog, this->assetData);
    }
}
