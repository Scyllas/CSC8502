# pragma once

#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/OBJmesh.h"
#include "../nclgl/heightmap.h"
#include "../nclgl/light.h"
#include "TextMesh.h"
#include <string>
using namespace std;
# define LIGHTNUM 8 // We ’ll generate LIGHTNUM squared lights ...

const int msecArraySize = 600;

class Renderer : public OGLRenderer {
public:
	Renderer(Window & parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	void DrawSkybox();
	void GenerateText();
	virtual void UpdateScene(float msec);

protected:
	void FillBuffers(); //G- Buffer Fill Render Pass
	void DrawPointLights(); // Lighting Render Pass
	void CombineBuffers(); // Combination Render Pass
	void DrawWater();
	void DrawText(const std::string &text, const Vector3 &position, const float size = 10.0f, const bool perspective = false);
	void storeMsec(float msec);
	std::string getFPS();
						   // Make a new texture ...
	void GenerateScreenTexture(GLuint & into, bool depth = false);
	Shader * sceneShader; // Shader to fill our GBuffers
	Shader * pointlightShader; // Shader to calculate lighting
	Shader * combineShader; // shader to stick it all together
	Shader * reflectShader;
	Shader * textShader;
	Shader * skyboxShader;
	Shader * lightShader;

	Light * pointLights; // Array of lighting data
	Light * light;
	OBJMesh * sphere; // Light volume
	Mesh * heightMap; // Terrain !
	Mesh * quad; // To draw a full - screen quad
	Mesh * water; // To draw a full - screen quad
	Mesh * skyBox; // To draw a full - screen quad
	Camera * camera; // Our usual camera

	float rotation; // How much to increase rotation by
	float waterRotate;
	
	int msecArrayLength = 0;
	int msecArrayLocation = 0;
	float msecArray[msecArraySize];

	GLuint bufferFBO; // FBO for our G- Buffer pass
	GLuint bufferColourTex; // Albedo goes here
	GLuint bufferNormalTex; // Normals go here
	GLuint bufferDepthTex; // Depth goes here

	GLuint pointLightFBO; // FBO for our lighting pass
	GLuint lightEmissiveTex; // Store emissive lighting
	GLuint lightSpecularTex; // Store specular lighting

	GLuint cubeMap;

	Font*	basicFont;	//A font! a basic one...
};