# pragma once

# include "../NCLGL/OGLRenderer.h"
# include "../NCLGL/Camera.h"
# include "../NCLGL/MD5Mesh.h"

class Renderer : public OGLRenderer {
public:
	Renderer(Window & parent);
	virtual ~Renderer(void);
	virtual void RenderScene();
	virtual void UpdateScene(float msec);
protected:
	MD5FileData * hellData;
	MD5Node * hellNode;
	Camera * camera;
};