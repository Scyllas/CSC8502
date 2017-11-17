#pragma once

#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/OBJmesh.h"
#include "../nclgl/heightmap.h"
#include "../nclgl/light.h"

#include "TextMesh.h"
#include "SharedFunctions.h"

class SceneB : public OGLRenderer {
public:
	SceneB(Window & parent);
	virtual ~SceneB(void) {};

	void destroy();

	void UpdateScene(float msec);
	void RenderScene();

	void GenerateScreenTexture(GLuint & into, bool depth = false);
protected:

	Camera * camera; // Our usual camera
};

