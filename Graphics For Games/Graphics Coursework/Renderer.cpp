# include"Renderer.h"


Renderer::Renderer(Window & parent) : OGLRenderer(parent) {
	rotation = 0.0f;
	camera = new Camera(0.0f, 0.0f,
		Vector3(RAW_WIDTH * HEIGHTMAP_X / 2.0f, 500, RAW_WIDTH * HEIGHTMAP_X));

	quad = Mesh::GenerateQuad();
	water = Mesh::GenerateQuad();
	skyBox = Mesh::GenerateQuad();

	pointLights = new Light[LIGHTNUM * LIGHTNUM];
	for (int x = 0; x < LIGHTNUM; ++x) {
		for (int z = 0; z < LIGHTNUM; ++z) {
			Light & l = pointLights[(x * LIGHTNUM) + z];

			float xPos = (RAW_WIDTH * HEIGHTMAP_X / (LIGHTNUM - 1)) * x;
			float zPos = (RAW_HEIGHT * HEIGHTMAP_Z / (LIGHTNUM - 1)) * z;
			l.SetPosition(Vector3(xPos, 100.0f, zPos));

			float r = 0.5f + (float)(rand() % 129) / 128.0f;
			float g = 0.5f + (float)(rand() % 129) / 128.0f;
			float b = 0.5f + (float)(rand() % 129) / 128.0f;
			l.SetColour(Vector4(r, g, b, 1.0f));

			float radius = (RAW_WIDTH * HEIGHTMAP_X / LIGHTNUM);
			l.SetRadius(radius);

		}

	}

	light = new Light(
		Vector3((RAW_HEIGHT * HEIGHTMAP_X / 2.0f), 500.0f,(RAW_HEIGHT * HEIGHTMAP_Z / 2.0f)),
		Vector4(0.9f, 0.9f, 1.0f, 1),
		(RAW_WIDTH * HEIGHTMAP_X) / 2.0f);

	water->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Water.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	basicFont = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);
	heightMap = new HeightMap(TEXTUREDIR"terrain.raw");
	heightMap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"Barren RedsDOT3.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	cubeMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR"rusted_west.jpg", TEXTUREDIR"rusted_east.jpg",
		TEXTUREDIR"rusted_up.jpg", TEXTUREDIR"rusted_down.jpg",
		TEXTUREDIR"rusted_south.jpg", TEXTUREDIR"rusted_north.jpg",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID, 0
	);
	skyBox->SetTexture(cubeMap);

	SetTextureRepeating(heightMap->GetTexture(), true);
	SetTextureRepeating(heightMap->GetBumpMap(), true);
	SetTextureRepeating(water->GetTexture(), true);

	if (!cubeMap) {
		return;
	}
	sphere = new OBJMesh();
	if (!sphere->LoadOBJMesh(MESHDIR"ico.obj")) {
		return;
	}
	textShader = new Shader(SHADERDIR"TextVertex.glsl", SHADERDIR"TextFragment.glsl");
	if (!textShader->LinkProgram()) {
		return;
	}
	sceneShader = new Shader(SHADERDIR"BumpVertex.glsl", SHADERDIR"bufferFragment.glsl");
	if (!sceneShader->LinkProgram()) {
		return;
	}
	combineShader = new Shader(SHADERDIR"combinevert.glsl", SHADERDIR"combinefrag.glsl");
	if (!combineShader->LinkProgram()) {
		return;
	}
	pointlightShader = new Shader(SHADERDIR"pointlightvert.glsl", SHADERDIR"pointlightfrag.glsl");
	if (!pointlightShader->LinkProgram()) {
		return;
	}
	reflectShader = new Shader(SHADERDIR"PerPixelVert.glsl", SHADERDIR"reflectFragment.glsl");
	if (!reflectShader->LinkProgram()) {
		return;
	}
	skyboxShader = new Shader(SHADERDIR"skyboxVertex.glsl",	SHADERDIR"skyboxFragment.glsl");
	if (!skyboxShader->LinkProgram()) {
		return;
	}
	lightShader = new Shader(SHADERDIR"PerPixelVert.glsl",	SHADERDIR"PerPixelFragment.glsl");
	if (!lightShader->LinkProgram()) {
		return;
	}
	glGenFramebuffers(1, &bufferFBO);
	glGenFramebuffers(1, &pointLightFBO);

	GLenum buffers[2];
	buffers[0] = GL_COLOR_ATTACHMENT0;
	buffers[1] = GL_COLOR_ATTACHMENT1;

	//Generate our scene depth texture...
	GenerateScreenTexture(bufferDepthTex, true);
	GenerateScreenTexture(bufferColourTex);
	GenerateScreenTexture(bufferNormalTex);
	GenerateScreenTexture(lightEmissiveTex);
	GenerateScreenTexture(lightSpecularTex);
	//And now attach them to our FBOs
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, bufferColourTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
		GL_TEXTURE_2D, bufferNormalTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, bufferDepthTex, 0);
	glDrawBuffers(2, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
		GL_FRAMEBUFFER_COMPLETE) {
		return;

	}
	glBindFramebuffer(GL_FRAMEBUFFER, pointLightFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, lightEmissiveTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
		GL_TEXTURE_2D, lightSpecularTex, 0);
	glDrawBuffers(2, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
		GL_FRAMEBUFFER_COMPLETE) {
		return;

	}
	waterRotate = 0.0f;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	init = true;

}
Renderer ::~Renderer(void) {
	delete sceneShader;
	delete combineShader;
	delete pointlightShader;
	delete reflectShader;
	delete textShader;
	delete skyboxShader;
	delete lightShader;

	delete basicFont;
	delete heightMap;
	delete camera;
	delete sphere;
	delete quad;
	delete water;
	delete skyBox;
	delete[] pointLights;
	delete light;

	glDeleteTextures(1, &bufferColourTex);
	glDeleteTextures(1, &bufferNormalTex);
	glDeleteTextures(1, &bufferDepthTex);
	glDeleteTextures(1, &lightEmissiveTex);
	glDeleteTextures(1, &lightSpecularTex);

	glDeleteFramebuffers(1, &bufferFBO);
	glDeleteFramebuffers(1, &pointLightFBO);
	currentShader = 0;

}
void Renderer::GenerateScreenTexture(GLuint & into, bool depth) {
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

void Renderer::UpdateScene(float msec) {
	storeMsec(msec);
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	rotation = msec * 0.01f;
	waterRotate += msec / 1000.0f;

}

void Renderer::RenderScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);


	FillBuffers();
	DrawSkybox();
	DrawPointLights();
	CombineBuffers();

	GenerateText();
	SwapBuffers();
	

}

void Renderer::DrawSkybox() {

	glDepthMask(GL_FALSE);
	SetCurrentShader(skyboxShader);
	glUseProgram(currentShader->GetProgram());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"cubeTex"), 2);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	UpdateShaderMatrices();
	skyBox->Draw();

	glUseProgram(0);
	glDepthMask(GL_TRUE);

}



void Renderer::FillBuffers() {
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	SetCurrentShader(sceneShader);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"bumpTex"), 1);


	projMatrix = Matrix4::Perspective(1.0f, 10000.0f,
		(float)width / (float)height, 45.0f);
	modelMatrix.ToIdentity();
	UpdateShaderMatrices();
	
	heightMap->Draw();
	DrawWater();

	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Renderer::DrawPointLights() {

	SetCurrentShader(pointlightShader);
	glUseProgram(currentShader->GetProgram());

	glBindFramebuffer(GL_FRAMEBUFFER, pointLightFBO);

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glBlendFunc(GL_ONE, GL_ONE);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"depthTex"), 3);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"normTex"), 4);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTex);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, bufferNormalTex);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float *)& camera->GetPosition());

	glUniform2f(glGetUniformLocation(currentShader->GetProgram(),
		"pixelSize"), 1.0f / width, 1.0f / height);
	Vector3 translate = Vector3((RAW_HEIGHT * HEIGHTMAP_X / 2.0f), 500,
		(RAW_HEIGHT * HEIGHTMAP_Z / 2.0f));

	Matrix4 pushMatrix = Matrix4::Translation(translate);
	Matrix4 popMatrix = Matrix4::Translation(-translate);

	for (int x = 0; x < LIGHTNUM; ++x) {
		for (int z = 0; z < LIGHTNUM; ++z) {
			Light & l = pointLights[(x * LIGHTNUM) + z];
			float radius = l.GetRadius();

			modelMatrix =
				pushMatrix *
				Matrix4::Rotation(rotation, Vector3(0, 1, 0)) *
				popMatrix *
				Matrix4::Translation(l.GetPosition()) *
				Matrix4::Scale(Vector3(radius, radius, radius));

			l.SetPosition(modelMatrix.GetPositionVector());

			SetShaderLight(l);

			UpdateShaderMatrices();

			float dist = (l.GetPosition() - camera->GetPosition()).Length();
			if (dist < radius) {//camera is inside the light volume !
				glCullFace(GL_FRONT);

			}
			else {
				glCullFace(GL_BACK);

			}

			sphere->Draw();

		}

	}
	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.2f, 0.2f, 0.2f, 1);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);

}

void Renderer::CombineBuffers() {

	SetCurrentShader(combineShader);
	glUseProgram(currentShader->GetProgram());

	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 2);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"emissiveTex"), 3);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"specularTex"), 4);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, bufferColourTex);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, lightEmissiveTex);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, lightSpecularTex);

	quad->Draw();
	
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::DrawWater() {
	//SetCurrentShader(reflectShader);
	//SetShaderLight(*light);
	/*glUniform3fv(glGetUniformLocation(currentShader->GetProgram(),
		"cameraPos"), 1, (float *)& camera->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),
		"diffuseTex"), 0);
*/
	

	float heightX = (RAW_WIDTH * HEIGHTMAP_X / 2.0f);

	float heightY = 256 * HEIGHTMAP_Y / 3.0f;

	float heightZ = (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f);

	modelMatrix =
		Matrix4::Translation(Vector3(heightX, heightY, heightZ)) *
		Matrix4::Scale(Vector3(heightX, 1, heightZ)) *
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	/*	textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) *
			Matrix4::Rotation(waterRotate, Vector3(0.0f, 0.0f, 1.0f));
*/			
	
	UpdateShaderMatrices();

	water->Draw();

	glUseProgram(0);

}

void Renderer::GenerateText() {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	SetCurrentShader(textShader);
	glUseProgram(currentShader->GetProgram());	//Enable the shader...
												//And turn on texture unit 0
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	//Render function to encapsulate our font rendering!
	DrawText(getFPS(), Vector3(0, 0, 0), 16.0f);
	//DrawText("This is perspective text!!!!", Vector3(0, 0, -1000), 64.0f, true);

	glUseProgram(0);	//That's everything!
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void Renderer::storeMsec(float msec) {

	msecArrayLocation %= msecArraySize;
	msecArray[msecArrayLocation] = msec;
	msecArrayLocation++;
	if (msecArrayLength < msecArraySize) msecArrayLength++;
}
std::string Renderer::getFPS() {

	float totalTime = 0.0f;
	float fps;
	for (int i = 0; i < msecArrayLength; i++) {
		totalTime += msecArray[i];
	}

	fps = 1000.f / (totalTime / msecArrayLength);
	return to_string(fps);
}

void Renderer::DrawText(const std::string &text, const Vector3 &position, const float size, const bool perspective) {
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
