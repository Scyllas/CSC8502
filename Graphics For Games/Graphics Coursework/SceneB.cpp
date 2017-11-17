#include "SceneB.h"

SceneB::SceneB(Window & parent) : OGLRenderer(parent)
{

	camera = new Camera(0.0f, 0.0f,
		Vector3(RAW_WIDTH * HEIGHTMAP_X / 2.0f, 500, RAW_WIDTH * HEIGHTMAP_X));
}

void SceneB::destroy()
{
}

void SceneB::UpdateScene(float msec)
{
}

void SceneB::RenderScene()
{
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



