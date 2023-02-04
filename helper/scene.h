#pragma once

#include <glm/glm.hpp>
#include "../Camera.h"

class Scene
{
public:
    int width;
    int height;
    Camera cam;
    float lastUpdateTime = 0;

	Scene() : m_animate(true), width(800), height(600) {
        this->cam.aspect = width / height;
    }

    Scene(int width, int height) : m_animate(true) {
        this->width = width;
        this->height = height;
        this->cam.aspect = width / height;
    }

	virtual ~Scene() {}

    virtual void keyDown(int key) = 0;

	void setDimensions( int w, int h ) {
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
    virtual void update( float t ) = 0;

    /**
      Draw your scene.
      */
    virtual void render() = 0;

    /**
      Called when screen is resized
      */
    virtual void resize(int, int) = 0;
    
    void animate( bool value ) { m_animate = value; }
    bool animating() { return m_animate; }
    
protected:
	bool m_animate;
};
