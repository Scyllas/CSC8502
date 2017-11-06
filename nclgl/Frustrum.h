# pragma once

# include "Plane.h"
# include "Matrix4.h"
# include "SceneNode.h"
class Matrix4; //Compile the Mat4 class first , please !

class Frustrum {
public:
	Frustrum(void) {};
	~Frustrum(void) {};

	void FromMatrix(const Matrix4 & mvp);
	bool InsideFrustum(SceneNode & n);
protected:
	Plane planes[6];

};