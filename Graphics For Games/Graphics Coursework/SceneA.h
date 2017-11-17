#pragma once

#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/OBJmesh.h"
#include "../nclgl/heightmap.h"
#include "../nclgl/light.h"

#include "TextMesh.h"
#include "SharedFunctions.h"

# define LIGHTNUM 8 // We ’ll generate LIGHTNUM squared lights ...

class SceneA : public OGLRenderer {
public:


	SceneA(Window & parent);
	virtual ~SceneA(void) {};

	void destroy();

	void UpdateScene(float msec);
	void RenderScene();

	Shader * sceneShader; // Shader to fill our GBuffers
	Shader * pointlightShader; // Shader to calculate lighting
	Shader * combineShader; // shader to stick it all together
	Shader * textShader;
	Shader * lightShader;

	Light * pointLights; // Array of lighting data
	OBJMesh * sphere; // Light volume
	Mesh * heightMap; // Terrain !
	Mesh * quad; // To draw a full - screen quad
	Mesh * water; // To draw a full - screen quad

	float rotation; // How much to increase rotation by
	float waterRotate;

	GLuint bufferFBO; // FBO for our G- Buffer pass
	GLuint bufferColourTex; // Albedo goes here
	GLuint bufferNormalTex; // Normals go here
	GLuint bufferDepthTex; // Depth goes here

	GLuint pointLightFBO; // FBO for our lighting pass
	GLuint lightEmissiveTex; // Store emissive lighting
	GLuint lightSpecularTex; // Store specular lighting

	Font*	basicFont;	//A font! a basic one...
protected:

	void GenerateScreenTexture(GLuint & into, bool depth = false);

	void FillBuffers();

	void DrawWater();

	void DrawPointLights();

	void CombineBuffers();

	void GenerateText();

	void DrawText(const std::string &text, const Vector3 &position, const float size = 10.0f, const bool perspective = false);

	Camera * camera; // Our usual camera
};

