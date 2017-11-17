# pragma once


#include <string>

#include "SceneA.h"
#include "SceneB.h"
#include "SceneC.h"

using namespace std;

class Renderer {
public:
	Renderer(Window & parent);
	virtual ~Renderer(void);


	virtual void RenderScene(bool flag);
	bool HasInitialised();
	virtual void UpdateScene(float msec);

protected:
	
	// Make a new texture ...
	
	SceneA * A;
	SceneB * B;
	//SceneC C;
};