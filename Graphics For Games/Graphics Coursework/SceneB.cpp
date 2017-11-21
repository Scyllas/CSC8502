#include "SceneB.h"

SceneB::SceneB(Window & parent) : OGLRenderer(parent)
{


	camera = new Camera();
	camera->SetPosition(Vector3(0, 0, 100));
	walls.push_back(Mesh::GenerateQuad(1000.0f, 500.0f));
	walls.push_back(Mesh::GenerateQuad(1000.0f, 500.0f));
	walls.push_back(Mesh::GenerateQuad(1000.0f, 500.0f));
	walls.push_back(Mesh::GenerateQuad(250.0f, 500.0f));
	walls.push_back(Mesh::GenerateQuad(250.0f, 500.0f));
	walls.push_back(Mesh::GenerateQuad(1000.0f, 1000.0f));
	walls.push_back(Mesh::GenerateQuad(1000.0f, 1000.0f));
	walls.push_back(Mesh::GenerateQuad(2000.0f, 500.0f));
	walls.push_back(Mesh::GenerateQuad(2000.0f, 500.0f));
	walls.push_back(Mesh::GenerateQuad(500.0f, 2000.0f));
	walls.push_back(Mesh::GenerateQuad(500.0f, 2000.0f));



	lights = new Light[lightNum];
	lights[0] = Light(Vector3(0, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 5000.0f);
	lights[1] = Light(Vector3(0, 0.0f, -2000.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 5000.0f);

	torch1Particles = new ParticleEmitter();

	//pillar = OBJMesh(MESHDIR"simple_pillar.obj");
	//column = OBJMesh(MESHDIR"Column_Made_By_Tyro_Smith.obj");
	//torch = OBJMesh(MESHDIR"walltorch.obj");

	for (Mesh* it : walls) {
		it->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"mossyBrick.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		it->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"mossyBrickBump.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		SetTextureRepeating(it->GetTexture(), true);
	}

	basicFont = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);

	if (!cubeMap) {
		return;
	}
	wallShader = new Shader(SHADERDIR"basicVertex.glsl", SHADERDIR"TexturedFragment.glsl");
	if (!wallShader->LinkProgram()) {
		return;
	}
	textShader = new Shader(SHADERDIR"TextVertex.glsl", SHADERDIR"TextFragment.glsl");
	if (!textShader->LinkProgram()) {
		return;
	}
	lightShader = new Shader(SHADERDIR"PerPixelVert.glsl", SHADERDIR"PerPixelFragment.glsl");
	if (!lightShader->LinkProgram()) {
		return;
	}
	particleShader = new Shader(SHADERDIR"particleVertex.glsl", SHADERDIR"particleFragment.glsl", SHADERDIR"particleGeometry.glsl");
	if (!particleShader->LinkProgram()) {
		return;
	}
	glClearColor(0.0f, 0.0f, 0.0f, 1);

	projMatrix = Matrix4::Perspective(1.0f, 15000.0f,
		(float)width / (float)height, 45.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	repeating = false;


	init = true;
}

void SceneB::destroy()
{
	delete torch1Particles;

	walls.clear();

	delete lights;
	delete camera;

	delete wallShader;
	delete textShader;
	delete lightShader;
	delete particleShader;

	currentShader = 0;
}

void SceneB::UpdateScene(float msec)
{
	storeMsec(msec);
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	torch1Particles->Update(msec);
}

void SceneB::RenderScene()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	projMatrix = Matrix4::Perspective(1.0f, 15000.0f,
		(float)width / (float)height, 45.0f);

	GenerateWalls();
	//GenerateScenery();
	GenerateParticles();

	//	GenerateText();
	SwapBuffers();
}

void SceneB::GenerateWalls()
{

	modelMatrix.ToIdentity();

	SetCurrentShader(lightShader);


	repeating = true;

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float *)& camera->GetPosition());

	UpdateShaderMatrices();
	SetShaderLights(lights, lightNum);

	vector<Mesh*>::iterator it = walls.begin();
	modelMatrix =//back face
		Matrix4::Translation(Vector3(0.0f, 0.0f, -2500.0f))*
		Matrix4::Rotation(0, Vector3(0.0f, 0.0f, 0.0f));
	UpdateShaderMatrices();
	(*it)->Draw();
	it++;

	modelMatrix =//left face
		Matrix4::Translation(Vector3(-500.0f, 0.0f, -2000.0f))*
		Matrix4::Rotation(90, Vector3(0.0f, 1.0f, 0.0f));
	UpdateShaderMatrices();
	(*it)->Draw();
	it++;

	modelMatrix =//right face
		Matrix4::Translation(Vector3(500.0f, 0.0f, -2000.0f))*
		Matrix4::Rotation(90, Vector3(0.0f, -1.0f, 0.0f));
	UpdateShaderMatrices();
	(*it)->Draw();
	it++;

	modelMatrix =//front face left
		Matrix4::Translation(Vector3(-375.0f, 0.0f, -1500.0f))*
		Matrix4::Rotation(180, Vector3(0.0f, 1.0f, 0.0f));
	UpdateShaderMatrices();
	(*it)->Draw();
	it++;

	modelMatrix =//front face right
		Matrix4::Translation(Vector3(375.0f, 0.0f, -1500.0f))*
		Matrix4::Rotation(180, Vector3(0.0f, 1.0f, 0.0f));
	UpdateShaderMatrices();
	(*it)->Draw();
	it++;

	modelMatrix =//top face
		Matrix4::Translation(Vector3(0.0f, 250.0f, -2000.0f))*
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));
	UpdateShaderMatrices();
	(*it)->Draw();
	it++;

	modelMatrix =//bottom face
		Matrix4::Translation(Vector3(0.0f, -250.0f, -2000.0f))*
		Matrix4::Rotation(90, Vector3(-1.0f, 0.0f, 0.0f));
	UpdateShaderMatrices();
	(*it)->Draw();
	it++;
	modelMatrix =//hallway left
		Matrix4::Translation(Vector3(-250.0f, 0.0f, -500.0f))*
		Matrix4::Rotation(90, Vector3(0.0f, -1.0f, 0.0f));
	UpdateShaderMatrices();
	(*it)->Draw();
	it++;
	modelMatrix =//hallway right
		Matrix4::Translation(Vector3(250.0f, 0.0f, -500.0f))*
		Matrix4::Rotation(90, Vector3(0.0f, 1.0f, 0.0f));
	UpdateShaderMatrices();
	(*it)->Draw();
	it++;
	modelMatrix =//top hall
		Matrix4::Translation(Vector3(0.0f, 250.0f, -500.0f))*
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));
	UpdateShaderMatrices();
	(*it)->Draw();
	it++;

	modelMatrix =//bottom hall
		Matrix4::Translation(Vector3(0.0f, -250.0f, -500.0f))*
		Matrix4::Rotation(90, Vector3(-1.0f, 0.0f, 0.0f));
	UpdateShaderMatrices();
	(*it)->Draw();
	it++;


	UpdateShaderMatrices();
	repeating = false;
	modelMatrix.ToIdentity();
	glUseProgram(0);

}

void SceneB::GenerateScenery()
{
	modelMatrix.ToIdentity();

	SetCurrentShader(wallShader);



	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	modelMatrix =//hallway left
		Matrix4::Translation(Vector3(0.0f, 0.0f, 0.0f))*
		Matrix4::Scale(Vector3(1000.0f, 1000.0f, 1000.0f));
	UpdateShaderMatrices();

	pillar.Draw();

	//column.Draw();
	//torch.Draw();

	modelMatrix.ToIdentity();
	glUseProgram(0);
}

void SceneB::GenerateParticles() {


	SetCurrentShader(particleShader);

	modelMatrix = Matrix4::Translation(Vector3(0.0f, 0.0f, -500.0f));

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	torch1Particles->SetParticleSize(40.0f);
	torch1Particles->SetParticleVariance(1.0f);
	torch1Particles->SetLaunchParticles(16.0f);
	torch1Particles->SetParticleLifetime(2000.0f);
	torch1Particles->SetParticleSpeed(0.1f);

	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "particleSize"), torch1Particles->GetParticleSize());

	UpdateShaderMatrices();

	torch1Particles->Draw();

	modelMatrix.ToIdentity();
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
