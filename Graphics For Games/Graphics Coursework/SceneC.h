#pragma once

#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/OBJmesh.h"
#include "../nclgl/heightmap.h"
#include "../nclgl/light.h"

#include "TextMesh.h"
#include "SharedFunctions.h"

class SceneC : public OGLRenderer {

public:
	SceneC(Window & parent);
	~SceneC() {};

	void GenerateScreenTexture(GLuint & into, bool depth = false);
	Camera * camera; // Our usual camera
};

