#include "scene.h"

void Scene::processKeys(float dt)
{
	while (!pendingKeys.empty())
	{
		ivec2 k = pendingKeys.back();
		pendingKeys.pop_back();
		//printf("\rKey Pressed %c                         \n\r", k[0]);
		keyActve(k[0], k[1], dt);
	}
}

void Scene::glKeyDown(int key, int mods) {
	pendingKeys.push_back(ivec2(key, mods));
}

void Scene::glKeyHeld(int key, int mods) {
	for (int i = 0; i < pendingKeys.size(); i++) {
		ivec2 k = pendingKeys[i];
		if (k[0] == key && k[1] == mods) return;
	}

	pendingKeys.push_back(ivec2(key, mods));
}

void Scene::glKeyReleased(int key, int mods) {
}