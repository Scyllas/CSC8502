#include "SceneC.h"


SceneC::SceneC(Window & parent) : OGLRenderer(parent)
{

	camera = new Camera(0.0f, 0.0f,
		Vector3(RAW_WIDTH * HEIGHTMAP_X / 2.0f, 500, RAW_WIDTH * HEIGHTMAP_X));
}



