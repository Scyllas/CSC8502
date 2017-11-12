#pragma once

#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/OBJmesh.h"
#include "../nclgl/heightmap.h"
#include "../nclgl/light.h"


class Renderer : public OGLRenderer {
public:
	Renderer(Window & parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

private:

	void GenerateScreenTexture(GLuint & into, bool depth = false);

	float rotation; // How much to increase rotation by
	Camera * camera; // Our usual camera
	Mesh * heightMap; // Terrain !

	Shader * sceneShader; // Shader to fill our GBuffers
	Shader * combineShader; // shader to stick it all together

	GLuint bufferFBO; // FBO for our G- Buffer pass
	GLuint bufferColourTex; // Albedo goes here
	GLuint bufferNormalTex; // Normals go here
	GLuint bufferDepthTex; // Depth goes here
};