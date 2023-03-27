#include "Asset.h"

#include <iostream>
#include<fstream>
#include<sstream>
#include<string>
#include <algorithm>

inline string GetFormattedLengthString(int length) {
	string old_str = to_string(length);
	const size_t n_zero = 8;

	auto new_str = std::string(n_zero - std::min(n_zero, old_str.length()), '0') + old_str;

	return string(new_str);
}

template<typename T>
inline void DumpVertex(ofstream* fileStr, std::vector<T>* vec) {
	string len = GetFormattedLengthString(vec->size());
	fileStr->write(len.c_str(), len.size());

	fileStr->write((char*)vec->data(), vec->size() * sizeof(T));
}

void Mesh::Dump(ofstream* fileStr)
{
	string nameLen = GetFormattedLengthString(name.size());
	fileStr->write(nameLen.c_str(), nameLen.size());

	fileStr->write(name.c_str(), name.size());

	DumpVertex(fileStr, &this->data->vertexSet);
	DumpVertex(fileStr, &this->data->texCooSet);
	DumpVertex(fileStr, &this->data->normalSet);

	string total = to_string(this->data->vertexSet.size() + this->data->texCooSet.size() + this->data->normalSet.size());

	printf("Dumped Mesh %s With %s elements\n", name.c_str(), total.c_str());
}

template<typename T>
int BuildVector(vector<T>* vec, char* data, int idx) {
	string lenStr;
	memcpy((void*)lenStr.data(), &data[idx], 8);

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
			string lenStr;
			memcpy((void*)lenStr.data(), &str_buff[idx], 8);

			int len = stoi(lenStr);

			idx += 8;

			string name;
			memcpy((void*)name.data(), &str_buff[idx], len);

			idx += len;

			Mesh m(name);
			m.data = new MeshData();

			idx += BuildVector(&m.data->vertexSet, str_buff, idx);

			idx += BuildVector(&m.data->texCooSet, str_buff, idx);

			idx += BuildVector(&m.data->normalSet, str_buff, idx);

			this->meshses.push_back(m);

			string total = to_string(m.data->vertexSet.size() + m.data->texCooSet.size() + m.data->normalSet.size());

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
		this->meshses[i].Dump(&fileStr);
	}

	fileStr.flush();
	fileStr.close();
}