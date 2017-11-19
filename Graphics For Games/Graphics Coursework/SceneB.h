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
	void GenerateText();
	void DrawText(const std::string &text, const Vector3 &position, const float size = 10.0f, const bool perspective = false);
protected:

	GLuint bufferFBO; // FBO for our G- Buffer pass
	GLuint bufferColourTex; // Albedo goes here
	GLuint bufferNormalTex; // Normals go here
	GLuint bufferDepthTex; // Depth goes here

	GLuint pointLightFBO; // FBO for our lighting pass
	GLuint lightEmissiveTex; // Store emissive lighting
	GLuint lightSpecularTex; // Store specular lighting

	Shader * textShader;
	Font*	basicFont;	//A font! a basic one...
	Mesh * heightMap; // Terrain !
	Camera * camera; // Our usual camera
};

