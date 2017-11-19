# include"Renderer.h"



Renderer::Renderer(Window & parent) {

	//A = new SceneA(parent);
	B = new SceneB(parent);
	//C = SceneC(parent);

}

Renderer ::~Renderer(void) {


	//delete A;
	delete B;
	//delete C;

}



void Renderer::UpdateScene(float msec) {




	if (m_flag == 1) {
		//A->UpdateScene(msec);
	}
	else if (m_flag == 2) {
		B->UpdateScene(msec);
	}
	else if (m_flag == 3) {
		//C->UpdateScene(msec);
	}
}

void Renderer::RenderScene(int flag) {

	m_flag = flag;

	if (flag == 1) {
		//A->RenderScene();
	}
	else if (flag == 2) {
		B->RenderScene();
	}
//	else if (flag == 3) {
//		C->RenderScene();
//	}




}

bool Renderer::HasInitialised() {

	//if (!A->HasInitialised()) {
//		return false;
//	}
	if (!B->HasInitialised()) {
		return false;
	}
//	if (!C.HasInitialised()) {
//		return false;
//	}

	return true;
}


