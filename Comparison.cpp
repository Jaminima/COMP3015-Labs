#include "Asset.h"

bool Asset::AssetEqual(Asset* a)
{
	int meshCount = this->meshses.size();
	for (int i = 0; i < meshCount; i++) {
		if (!this->meshses[i].MeshEqual(&a->meshses[i])) {
			return false;
		}
	}
}

template<typename T>
inline bool VectorEqual(vector<T>* vec1, vector<T>* vec2) {
	int vec1len = vec1->size();
	int vec2len = vec2->size();

	if (vec1len != vec2len) return false;

	for (int i = 0; i < vec1len; i++) {
		if (vec1->at(i) != vec2->at(i)) {
			return false;
		}
	}

	return true;
}

bool Mesh::MeshEqual(Mesh* m)
{
	if (!VectorEqual(&data->vertexSet, &m->data->vertexSet)) return false;

	if (!VectorEqual(&data->normalSet, &m->data->normalSet)) return false;

	if (!VectorEqual(&data->texCooSet, &m->data->texCooSet)) return false;

	return  true;
}