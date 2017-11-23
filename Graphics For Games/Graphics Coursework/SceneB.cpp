#include "SceneB.h"

SceneB::SceneB(Window & parent) : OGLRenderer(parent)
{

	root = new SceneNode();
	wallRoot = new SceneNode();
	wall = Mesh::GenerateQuad();
	camera = new Camera();
	Vector2 tempSize[11];
	Matrix4 tempModel[11];

	column =  new OBJMesh(MESHDIR"Column.obj");
	hellData = new MD5FileData(MESHDIR"hellknight.md5mesh");
	hellNode = new MD5Node(*hellData);

	torch1Particles = new ParticleEmitter();
	lights = new Light[lightNum];

	basicFont = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);


	root->AddChild(wallRoot);
	camera->SetPosition(Vector3(0, 0, 100));


	wallShader = new Shader(SHADERDIR"basicVertex.glsl", SHADERDIR"TexturedFragment.glsl");
	if (!wallShader->LinkProgram()) {
		return;
	}
	textShader = new Shader(SHADERDIR"TextVertex.glsl", SHADERDIR"TextFragment.glsl");
	if (!textShader->LinkProgram()) {
		return;
	}
	lightShader = new Shader(SHADERDIR"bumpVertex.glsl", SHADERDIR"PerPixelFragment.glsl");
	if (!lightShader->LinkProgram()) {
		return;
	}
	particleShader = new Shader(SHADERDIR"particleVertex.glsl", SHADERDIR"particleFragment.glsl", SHADERDIR"particleGeometry.glsl");
	if (!particleShader->LinkProgram()) {
		return;
	}
	hellKnightShader = new Shader(SHADERDIR"texturedVertex.glsl", SHADERDIR"texturedFragment.glsl");
	if (!hellKnightShader->LinkProgram()) {
		return;
	}

	tempSize[0] = Vector2(1000.0f / 2, 500.0f / 2);
	tempSize[1] = Vector2(1000.0f / 2, 500.0f / 2);
	tempSize[2] = Vector2(1000.0f / 2, 500.0f / 2);
	tempSize[3] = Vector2(250.0f / 2, 500.0f / 2);
	tempSize[4] = Vector2(250.0f / 2, 500.0f / 2);
	tempSize[5] = Vector2(1000.0f / 2, 1000.0f / 2);
	tempSize[6] = Vector2(1000.0f / 2, 1000.0f / 2);
	tempSize[7] = Vector2(2000.0f / 2, 500.0f / 2);
	tempSize[8] = Vector2(2000.0f / 2, 500.0f / 2);
	tempSize[9] = Vector2(500.0f / 2, 2000.0f / 2);
	tempSize[10] = Vector2(500.0f / 2, 2000.0f / 2);

	tempModel[0] =
		Matrix4::Translation(Vector3(0.0f, 0.0f, -2500.0f))*
		Matrix4::Rotation(180, Vector3(0.0f, 1.0f, 0.0f));
	tempModel[1] =//left face
		Matrix4::Translation(Vector3(-500.0f, 0.0f, -2000.0f))*
		Matrix4::Rotation(90, Vector3(0.0f, -1.0f, 0.0f));
	tempModel[2] =//right face
		Matrix4::Translation(Vector3(500.0f, 0.0f, -2000.0f))*
		Matrix4::Rotation(90, Vector3(0.0f, 1.0f, 0.0f));
	tempModel[3] =//front face left
		Matrix4::Translation(Vector3(-375.0f, 0.0f, -1500.0f))*
		Matrix4::Rotation(180, Vector3(0.0f, 1.0f, 0.0f));
	tempModel[4] =//front face right
		Matrix4::Translation(Vector3(375.0f, 0.0f, -1500.0f))*
		Matrix4::Rotation(180, Vector3(0.0f, 1.0f, 0.0f));
	tempModel[5] =//top face
		Matrix4::Translation(Vector3(0.0f, 250.0f, -2000.0f))*
		Matrix4::Rotation(90, Vector3(-1.0f, 0.0f, 0.0f));
	tempModel[6] =//bottom face
		Matrix4::Translation(Vector3(0.0f, -250.0f, -2000.0f))*
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));
	tempModel[7] =//hallway left
		Matrix4::Translation(Vector3(-250.0f, 0.0f, -500.0f))*
		Matrix4::Rotation(90, Vector3(0.0f, -1.0f, 0.0f));
	tempModel[8] =//hallway right
		Matrix4::Translation(Vector3(250.0f, 0.0f, -500.0f))*
		Matrix4::Rotation(90, Vector3(0.0f, 1.0f, 0.0f));
	tempModel[9] =//top hall
		Matrix4::Translation(Vector3(0.0f, 250.0f, -500.0f))*
		Matrix4::Rotation(90, Vector3(-1.0f, 0.0f, 0.0f));
	tempModel[10] =//bottom hall
		Matrix4::Translation(Vector3(0.0f, -250.0f, -500.0f))*
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	wall->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"mossyBrick.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	wall->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"mossyBrickBump.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	SetTextureRepeating(wall->GetTexture(), true);
	hellData->AddAnim(MESHDIR"walk7.md5anim");
	hellNode->PlayAnim(MESHDIR"walk7.md5anim");

	for (int i = 0; i < 11; ++i) {
		SceneNode * s = new SceneNode();
		s->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		s->SetModelScale(Vector3(tempSize[i].x, tempSize[i].y, 100.0f));
		s->SetTransform(tempModel[i]);
		s->SetBoundingRadius(5000.0f);
		s->SetMesh(wall);
		s->SetOverrideShader(lightShader);
		wallRoot->AddChild(s);

	}

	
	hellNode->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	hellNode->SetModelScale(Vector3(2.0f, 2.0f, 2.0f));
	hellNode->SetBoundingRadius(5000.0f);
	hellNode->SetTransform(Matrix4::Translation(Vector3(0.0f, -250.0f, -1000.0f)));
	hellNode->SetOverrideShader(hellKnightShader);
	root->AddChild(hellNode);
	
	SceneNode * columnNode = new SceneNode();
	columnNode->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	columnNode->SetModelScale(Vector3(100.0f, 100.0f, 100.0f));
	columnNode->SetBoundingRadius(5000.0f);
	columnNode->SetMesh(column);
	columnNode->SetOverrideShader(wallShader);
	wallRoot->AddChild(columnNode);
	
	lights[0] = Light(Vector3(0, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 50000.0f);
	lights[1] = Light(Vector3(0, 0.0f, -1000.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 50000.0f);

	glClearColor(0.0f, 0.0f, 0.0f, 1);

	projMatrix = Matrix4::Perspective(1.0f, 15000.0f,
		(float)width / (float)height, 45.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	init = true;
}

void SceneB::destroy()
{
	delete torch1Particles;

	delete hellData;
	delete hellNode;
	delete column;

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

	frameFrustrum.FromMatrix(projMatrix * viewMatrix);

	torch1Particles->Update(msec);
	hellNode->Update(msec);
	root->Update(msec);
}

void SceneB::BuildNodeLists(SceneNode * from) {
	if (frameFrustrum.InsideFrustrum(*from)) {
		Vector3 dir = from->GetWorldTransform().GetPositionVector() -
			camera->GetPosition();
		from->SetCameraDistance(Vector3::Dot(dir, dir));

		if (from->GetColour().w < 1.0f) {
			transparentNodeList.push_back(from);

		}
		else {
			nodeList.push_back(from);

		}

	}

	for (vector < SceneNode * >::const_iterator i =
		from->GetChildIteratorStart();
		i != from->GetChildIteratorEnd(); ++i) {
		BuildNodeLists((*i));

	}

}

void SceneB::SortNodeLists() {
	std::sort(transparentNodeList.begin(),
		transparentNodeList.end(),
		SceneNode::CompareByCameraDistance);
	std::sort(nodeList.begin(),
		nodeList.end(),
		SceneNode::CompareByCameraDistance);

}

void SceneB::ClearNodeLists() {
	transparentNodeList.clear();
	nodeList.clear();

}

void SceneB::DrawNodes() {
	for (vector < SceneNode * >::const_iterator i = nodeList.begin();
		i != nodeList.end(); ++i) {
		DrawNode((*i));

	}
	for (vector < SceneNode * >::const_reverse_iterator i =
		transparentNodeList.rbegin();
		i != transparentNodeList.rend(); ++i) {
		DrawNode((*i));

	}

}

void SceneB::DrawNode(SceneNode * n) {

	Shader * overrideShader = ((n->GetOverrideShader() != NULL) ? n->GetOverrideShader() : currentShader);

	glUseProgram(overrideShader->GetProgram());

	if (n->GetMesh()) {
		glUniformMatrix4fv(glGetUniformLocation(
			overrideShader->GetProgram(), "modelMatrix"), 1, false,
			(float *)&(n->GetWorldTransform()* Matrix4::Scale(n->GetModelScale())));

		glUniform4fv(glGetUniformLocation(overrideShader->GetProgram(),
			"nodeColour"), 1, (float *)& n->GetColour());

		glUniform1i(glGetUniformLocation(overrideShader->GetProgram(),
			"useTexture"), (int)n->GetMesh()->GetTexture());

		n->Draw();

	}

}

void SceneB::RenderScene()
{
	BuildNodeLists(root);
	SortNodeLists();

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	projMatrix = Matrix4::Perspective(1.0f, 15000.0f,
		(float)width / (float)height, 45.0f);

	PassShaderUniforms();

	DrawObjects();

	UpdateHellKnight();
	GenerateParticles();
	GenerateText();

	SwapBuffers();
	ClearNodeLists();
}

void SceneB::PassShaderUniforms() {

	SetCurrentShader(lightShader);

	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float *)& camera->GetPosition());

	SetShaderLights(lights, lightNum);

	//next shader

	SetCurrentShader(hellKnightShader);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	UpdateShaderMatrices();

	

	//next shader

	SetCurrentShader(wallShader);

	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	//next shader

	SetCurrentShader(particleShader);

	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "particleSize"), torch1Particles->GetParticleSize());

	//next shader

	SetCurrentShader(textShader);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	glUseProgram(0);
}

void SceneB::DrawObjects()
{
	SetCurrentShader(lightShader);

	UpdateShaderMatrices();

	DrawNodes();

	glUseProgram(0);
}


void SceneB::GenerateParticles() {

	glDepthMask(GL_FALSE);
	SetCurrentShader(particleShader);

	modelMatrix =
		Matrix4::Translation(Vector3(450.0f, 125.0f, -2000.0f));

	torch1Particles->SetParticleRate(50.0f);
	torch1Particles->SetDirection(Vector3(-0.2f, 1.0f, 0));
	torch1Particles->SetParticleSize(10.0f);
	torch1Particles->SetParticleVariance(0.5f);
	torch1Particles->SetLaunchParticles(16.0f);
	torch1Particles->SetParticleLifetime(2000.0f);
	torch1Particles->SetParticleSpeed(0.05f);
	torch1Particles->SetinitialColour(Vector3(1.0f, 0, 0));

	UpdateShaderMatrices();


	torch1Particles->Draw();

	glDepthMask(GL_TRUE);
	modelMatrix.ToIdentity();
	glUseProgram(0);
}

void SceneB::UpdateHellKnight()
{

	SetCurrentShader(lightShader);
	
	//if (hellKnightPos > -500.0f) {
	//	hellKnightPos -= 5.0f;
	//	hellKnightDirection = true;
	//}
	//else if (hellKnightPos < -2000.0f) {
	//	hellKnightPos += 5.0f;
	//	hellKnightDirection = false;
	//}
	//else {
	//	if (hellKnightDirection == true) {
	//		hellKnightPos -= 5.0f;
	//	}
	//	else {
	//		hellKnightPos += 5.0f;
	//	}
	//}

	//hellNode->SetTransform(Matrix4::Translation(Vector3(0.0f, 0.0f, hellKnightPos)));

	UpdateShaderMatrices();

	//hellNode->Draw(*this);

	glUseProgram(0);


}

void SceneB::GenerateText() {

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glDisable(GL_DEPTH_TEST);

	SetCurrentShader(textShader);
	//Enable the shader...
	
	//Render function to encapsulate our font rendering!
	DrawText(getFPS(), Vector3(0, 0, 0), 16.0f);

	glUseProgram(0);	//That's everything!
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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