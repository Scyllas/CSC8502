# pragma once
2
3 # include "./ nclgl / OGLRenderer .h"
4 # include "./ nclgl / camera .h"
5 # include "./ nclgl / HeightMap .h"
6
7 class Renderer : public OGLRenderer {
	8 public:
		9 Renderer(Window & parent);
		10 virtual ~Renderer(void);
		11
			12 virtual void RenderScene();
		13 virtual void UpdateScene(float msec);
		14
			15 protected:
				16 HeightMap * heightMap;
				17 Camera * camera;
				18
};