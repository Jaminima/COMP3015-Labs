#pragma once

#include <glm/glm.hpp>
#include "../SceneObjects.h"

class Scene
{
private:
	vector<ivec2> pendingKeys;

public:
	int width;
	int height;

	int cMouseX;
	int cMouseY;

	SceneObjects sceneObjects;

	int framesInSecond = 0;
	float lastUpdateTime = 0;

	Scene() : m_animate(true), width(800), height(600) {
		this->sceneObjects.cam.aspect = width / height;
	}

	Scene(int width, int height) : m_animate(true) {
		this->width = width;
		this->height = height;
		this->sceneObjects.cam.aspect = width / height;
	}

	virtual ~Scene() {}

	virtual void keyActve(int key, int mods, float dt) = 0;

	virtual void mouseMove(int x, int y) = 0;

	void processKeys(float dt);

	void glKeyDown(int key, int mods);

	void glKeyHeld(int key, int mods);

	void glKeyReleased(int key, int mods);

	void setDimensions(int w, int h) {
		width = w;
		height = h;
	}

	/**
	  Load textures, initialize shaders, etc.
	  */
	virtual void initScene() = 0;

	/**
	  This is called prior to every frame.  Use this
	  to update your animation.
	  */
	void updateTriggered(float dt) {
		update(dt);

		framesInSecond = (lastUpdateTime - dt < floorf(lastUpdateTime)) ? 0 : framesInSecond + 1;
		int frames = floorf(framesInSecond / (lastUpdateTime - floorf(lastUpdateTime)));

		printf("Update Delta %f ms %i fps       \r", dt * 1000, frames);
	}

	virtual void update(float dt) = 0;

	/**
	  Draw your scene.
	  */
	virtual void render() = 0;

	/**
	  Called when screen is resized
	  */
	virtual void resize(int, int) = 0;

	void animate(bool value) { m_animate = value; }
	bool animating() { return m_animate; }

protected:
	bool m_animate;

	GLuint frameBuffer, renderedTexture, depthBuffer;
};
