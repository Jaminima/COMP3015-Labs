#include "Asset.h"

#include <iostream>
#include<fstream>
#include<sstream>
#include<string>
#include <algorithm>

template<typename T>
inline void DumpVertex(ofstream* fileStr, std::vector<T>* vec) {
	fileStr->write((char*)vec->data(), vec->size() * sizeof(T));
}

inline string GetFormattedLengthString(int length) {
	string old_str = to_string(length);
	const size_t n_zero = 8;

	auto new_str = std::string(n_zero - std::min(n_zero, old_str.length()), '0') + old_str;

	return string(new_str);
}

void Mesh::Dump(ofstream* fileStr)
{
	string nameLen = GetFormattedLengthString(name.size());
	fileStr->write(nameLen.c_str(), nameLen.size());

	fileStr->write(name.c_str(), name.size());

	string len = GetFormattedLengthString(this->data->vertexSet.size());
	fileStr->write(len.c_str(), len.size());

	DumpVertex(fileStr, &this->data->vertexSet);
	DumpVertex(fileStr, &this->data->texCooSet);
	DumpVertex(fileStr, &this->data->normalSet);

	printf("Dumped Mesh %s With %s elements\n", name.c_str(), len.c_str());
}


template<typename T>
void BuildVector(vector<T>* vec, char* data, int dLen) {
	int i = 0;

	while (i < dLen) {
		T vect;

		memcpy(&vect, &data[i], sizeof(T));

		vec->push_back(vect);

		i += sizeof(T);
	}
}

bool Asset::TryLoadDump()
{
	string filePath = srcFile + ".dump";

	ifstream file(filePath);
	string str;
	if (file) {
		ifstream fileBin(filePath, ios::binary | ios::ate);
		int sz = fileBin.tellg();
		fileBin.close();

		printf("Loading Dump For File %s\n", srcFile.c_str());
		loadedFromDump = true;

		char* str_buff = new char[sz];

		file.read(str_buff, sz);

		int idx = 0;
		while (idx < sz * 0.9) {
			string lenStr;
			memcpy(lenStr.data(), &str_buff[idx], 8);

			int len = stoi(lenStr);

			idx += 8;

			string name;
			memcpy(name.data(), &str_buff[idx], len);

			idx += len;

			memcpy(lenStr.data(), &str_buff[idx], 8);
			len = stoi(lenStr);

			idx += 8;

			int vec3len = len * sizeof(vec3);
			int vec2len = len * sizeof(vec2);

			Mesh m(name);
			m.data = new MeshData();

			char* vecData = new char[vec3len];

			memcpy(vecData, &str_buff[idx], vec3len);
			BuildVector(&m.data->vertexSet, vecData, vec3len);
			idx += vec3len;

			memcpy(vecData, &str_buff[idx], vec2len);
			BuildVector(&m.data->texCooSet, vecData, vec2len);
			idx += vec2len;

			memcpy(vecData, &str_buff[idx], vec3len);
			BuildVector(&m.data->normalSet, vecData,vec3len);
			idx += vec3len;

			this->meshses.push_back(m);
		}

		file.close();

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

	ofstream fileStr(srcFile + ".dump");

	int meshCount = this->meshses.size();
	for (int i = 0; i < meshCount; i++) {
		this->meshses[i].Dump(&fileStr);
	}

	fileStr.flush();
	fileStr.close();
}