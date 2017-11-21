#pragma once

#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/OBJmesh.h"
#include "../nclgl/heightmap.h"
#include "../nclgl/light.h"
#include "../nclgl/ParticleEmitter.h"

#include "TextMesh.h"
#include "SharedFunctions.h"

#include <vector>

class SceneB : public OGLRenderer {

public:
	SceneB(Window & parent);
	virtual ~SceneB(void) {};

	void destroy();

	void UpdateScene(float msec);
	void RenderScene();

	void GenerateWalls();

	void GenerateScenery();

	void GenerateParticles();

	void GenerateScreenTexture(GLuint & into, bool depth = false);
	void GenerateText();
	void DrawText(const std::string &text, const Vector3 &position, const float size = 10.0f, const bool perspective = false);
protected:

	bool repeating;

	ParticleEmitter* torch1Particles;

	OBJMesh pillar;
	//OBJMesh column;
	//OBJMesh torch;

	Shader * textShader;
	Shader * particleShader;
	Shader * wallShader;
	Shader * lightShader;

	const int lightNum = 2;
	Light * lights;

	GLuint cubeMap;
	vector<Mesh*> walls;

	Font*	basicFont;	//A font! a basic one...

	Camera * camera; // Our usual camera
};

