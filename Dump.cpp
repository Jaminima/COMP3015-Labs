#include "Asset.h"

#include <iostream>
#include<fstream>
#include<sstream>
#include<string>
#include <algorithm>

#define lengthStringSize (size_t)8

inline string GetFormattedLengthString(int length) {
	string old_str = to_string(length);
	const size_t n_zero = 8;

	auto new_str = std::string(lengthStringSize - std::min(n_zero, old_str.length()), '0') + old_str;

	return string(new_str);
}

inline void WriteTextWithLen(ofstream* fileStr,string text) {
	string textLen = GetFormattedLengthString(text.size());
	fileStr->write(textLen.c_str(), textLen.size());
	fileStr->write(text.c_str(), text.size());
}

template<typename T>
inline void DumpVertex(ofstream* fileStr, std::vector<T>* vec) {
	string len = GetFormattedLengthString(vec->size());
	fileStr->write(len.c_str(), len.size());

	fileStr->write((char*)vec->data(), vec->size() * sizeof(T));
}

void Mesh::Dump(ofstream* fileStr, Mesh* parent)
{
	if (subMesh != 0x0) subMesh->Dump(fileStr, this);

	string n = parent != 0x0 ? parent->name + "-" + name : name;

	WriteTextWithLen(fileStr, n);

	WriteTextWithLen(fileStr, material);

	DumpVertex(fileStr, &this->data->vertexSet);
	DumpVertex(fileStr, &this->data->texCooSet);
	DumpVertex(fileStr, &this->data->normalSet);

	string total = to_string(this->data->vertexSet.size() + this->data->texCooSet.size() + this->data->normalSet.size());

	printf("Dumped Mesh %s With %s elements\n", name.c_str(), total.c_str());
}

template<typename T>
int BuildVector(vector<T>* vec, char* data, int idx) {
	string lenStr;
	memcpy(lenStr.data(), &data[idx], 8);

	int dLen = stoi(lenStr) * sizeof(T);

	int i = 0;

	while (i < dLen) {
		T vect;

		memcpy(&vect, &data[idx + 8 + i], sizeof(T));

		vec->push_back(vect);

		i += sizeof(T);
	}

	return dLen + 8;
}

Mesh* findOrNewMesh(vector<Mesh>* meshes, string name) {
	for (int i = 0; i < meshes->size(); i++) {
		Mesh* m = &meshes->at(i);
		if (m->name == name) {
			return m;
		}
	}
	return new Mesh(name);
}

inline string GetTextWithLength(char* strBuff, int startIdx) {
	string lenStr(lengthStringSize,'\0');
	memcpy(lenStr.data(), &strBuff[startIdx], lengthStringSize);

	int len = stoi(lenStr);

	string val(len, '\0');
	memcpy(val.data(), &strBuff[startIdx + lengthStringSize], len);

	return val;
}

bool Asset::TryLoadDump()
{
	string filePath = "./assets/dumps/" + srcFile + ".dump";

	ifstream file(filePath, ios::binary | ios::ate);
	if (file) {
		int sz = file.tellg();
		file.seekg(0);

		loadedFromDump = true;

		char* str_buff = new char[sz];

		file.read(str_buff, sz);

		file.close();

		int idx = 0;
		while (idx < sz * 0.9) {
			string name = GetTextWithLength(str_buff, idx);

			idx += name.size() + lengthStringSize;

			Mesh* m = new Mesh(name);
			delete m->components;

			string mat = GetTextWithLength(str_buff, idx);
			m->material = mat;

			idx += mat.size() + lengthStringSize;

			idx += BuildVector(&m -> data->vertexSet, str_buff, idx);

			idx += BuildVector(&m -> data->texCooSet, str_buff, idx);

			idx += BuildVector(&m -> data->normalSet, str_buff, idx);

			int name_seperator = name.find("-");

			if (name_seperator > 0) {
				m -> name = name.substr(name_seperator+1);
				
				string parentMeshName = name.substr(0, name_seperator);
				Mesh* parent = findOrNewMesh(&this->meshses, parentMeshName);

				parent->subMesh = new SubMesh(m, parent->subMesh);
				this->meshses.push_back(*parent);
			}
			else {
				this->meshses.push_back(*m);
			}

			string total = to_string(m->data->vertexSet.size() + m->data->texCooSet.size() + m->data->normalSet.size());

			printf("Loaded Dump For Mesh %s Containing %s Elements\n", name.c_str(), total.c_str());
		}

		return true;
	}
	else {
		return false;
	}
}

void Asset::Dump()
{
	if (loadedFromDump) {
		//printf("Skipping Dumping Of File %s\n", srcFile.c_str());
		return;
	}

	ofstream fileStr("./assets/dumps/" + srcFile + ".dump", ios::binary);

	int meshCount = this->meshses.size();
	for (int i = 0; i < meshCount; i++) {
		this->meshses[i].Dump(&fileStr, 0x0);
	}

	fileStr.flush();
	fileStr.close();
}