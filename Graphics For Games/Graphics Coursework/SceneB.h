#pragma once

#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/OBJmesh.h"
#include "../nclgl/heightmap.h"
#include "../nclgl/light.h"
#include "../nclgl/ParticleEmitter.h"
#include "../nclgl/MD5Node.h"
#include "../nclgl/Frustrum.h"

#include "TextMesh.h"
#include "SharedFunctions.h"

#include <vector>
#include <algorithm>

class SceneB : public OGLRenderer {

public:
	SceneB(Window & parent);
	virtual ~SceneB(void) {};

	void destroy();

	void UpdateScene(float msec);
	void BuildNodeLists(SceneNode * from);
	void SortNodeLists();
	void RenderScene();

	void PassShaderUniforms();

	void DrawObjects();

	void ClearNodeLists();

	void DrawNodes();

	void DrawNode(SceneNode * n);



	void GenerateScenery();

	void GenerateParticles();

	void GenerateHellKnight();

	void GenerateScreenTexture(GLuint & into, bool depth = false);
	void GenerateText();
	void DrawText(const std::string &text, const Vector3 &position, const float size = 10.0f, const bool perspective = false);
protected:


	ParticleEmitter* torch1Particles;

	//OBJMesh pillar;
	OBJMesh column;
	//OBJMesh torch;

	Shader * textShader;
	Shader * particleShader;
	Shader * wallShader;
	Shader * lightShader;
	Shader * hellKnightShader;

	MD5FileData * hellData;
	MD5Node * hellNode;

	const int lightNum = 2;
	Light * lights;

	GLuint cubeMap;
	Mesh * wall;
	//vector<Mesh*> walls;

	Font*	basicFont;	//A font! a basic one...

	Camera * camera; // Our usual camera

	SceneNode * root;	SceneNode * wallRoot;	Frustrum frameFrustrum;	vector <SceneNode*> transparentNodeList;
	vector <SceneNode*> nodeList;
};

