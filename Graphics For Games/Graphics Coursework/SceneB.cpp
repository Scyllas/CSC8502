#include "SceneB.h"

SceneB::SceneB(Window & parent) : OGLRenderer(parent)
{


	camera = new Camera();
	camera -> SetPosition(Vector3(RAW_WIDTH * HEIGHTMAP_X / 2.0f,
		500.0f, RAW_WIDTH * HEIGHTMAP_X));
	wall = Mesh::GenerateQuad();


	wall->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	basicFont = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);
	//heightMap = new HeightMap(TEXTUREDIR"terrain.raw");
//	heightMap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"grass.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
//	heightMap->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"grassBump.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));


	//SetTextureRepeating(heightMap->GetTexture(), true);
//	SetTextureRepeating(heightMap->GetBumpMap(), true);


	wallShader = new Shader(SHADERDIR"basicVertex.glsl", SHADERDIR"TexturedFragment.glsl");
	if (!wallShader->LinkProgram()) {
		return;
	}
	textShader = new Shader(SHADERDIR"TextVertex.glsl", SHADERDIR"TextFragment.glsl");
	if (!textShader->LinkProgram()) {
		return;
	}
	
	//SetTextureRepeating(wall -> GetTexture(), true);
	glClearColor(1.0f, 0.6f, 0.7f, 1);

	projMatrix = Matrix4::Perspective(1.0f, 15000.0f,
		(float)width / (float)height, 45.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	init = true;
}

void SceneB::destroy()
{
	delete camera;
	delete wall;
	delete heightMap;

	delete wallShader;
	delete textShader;

	currentShader = 0;
}

void SceneB::UpdateScene(float msec)
{
	storeMsec(msec);
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
}

void SceneB::RenderScene()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	//projMatrix = Matrix4::Perspective(1.0f, 10000.0f,
	//	(float)width / (float)height, 45.0f);


GenerateWalls();
	

	GenerateText();
	SwapBuffers();
}

void SceneB::GenerateWalls()
{

	SetCurrentShader(wallShader);

	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	wall->Draw();

	glUseProgram(0);

}



void SceneB::GenerateText() {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDisable(GL_DEPTH_TEST);
	
	SetCurrentShader(textShader);
	glUseProgram(currentShader->GetProgram());	//Enable the shader...
												//And turn on texture unit 0
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	//Render function to encapsulate our font rendering!
	DrawText(getFPS(), Vector3(0, 0, 0), 16.0f);

	glUseProgram(0);	//That's everything!
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void SceneB::GenerateScreenTexture(GLuint & into, bool depth)
{

	glGenTextures(1, &into);
	glBindTexture(GL_TEXTURE_2D, into);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0,
		depth ? GL_DEPTH_COMPONENT24 : GL_RGBA8,
		width, height, 0,
		depth ? GL_DEPTH_COMPONENT : GL_RGBA,
		GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

}


void SceneB::DrawText(const std::string &text, const Vector3 &position, const float size, const bool perspective) {
	//Create a new temporary TextMesh, using our line of text and our font
	TextMesh* mesh = new TextMesh(text, *basicFont);

	//This just does simple matrix setup to render in either perspective or
	//orthographic mode, there's nothing here that's particularly tricky.
	if (perspective) {
		modelMatrix = Matrix4::Translation(position) * Matrix4::Scale(Vector3(size, size, 1));
		viewMatrix = camera->BuildViewMatrix();
		projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
	}
	else {
		//In ortho mode, we subtract the y from the height, so that a height of 0
		//is at the top left of the screen, which is more intuitive
		//(for me anyway...)
		modelMatrix = Matrix4::Translation(Vector3(position.x, height - position.y, position.z)) * Matrix4::Scale(Vector3(size, size, 1));
		viewMatrix.ToIdentity();
		projMatrix = Matrix4::Orthographic(-1.0f, 1.0f, (float)width, 0.0f, (float)height, 0.0f);
	}
	//Either way, we update the matrices, and draw the mesh
	UpdateShaderMatrices();
	mesh->Draw();

	delete mesh; //Once it's drawn, we don't need it anymore!
}
