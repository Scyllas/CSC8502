#pragma comment(lib, "nclgl.lib")

#include "../../NCLGL/window.h"
#include "Renderer.h"


int main() {
	Window w("My coursework project", 1280, 720, false); //This is all boring win32 window creation stuff!
	if (!w.HasInitialised()) {
		return -1;
	}

	int flag = 1;
	srand((unsigned int)w.GetTimer()->GetMS() * 1000.0f);

	Renderer renderer(w); //This handles all the boring OGL 3.2 initialisation stuff, and sets up our tutorial!
	if (!renderer.HasInitialised()) {
		return -1;
	}


	//	w.LockMouseToWindow(true);
	//  w.ShowOSPointer(false);

	w.GetTimer()->GetTimedMS(); //Reset window timer to discard 'loading' time for first frame deltatime
	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_1)) {
			flag = 1;
		}
		else if (Window::GetKeyboard()->KeyDown(KEYBOARD_2)) {
			flag = 2;
		}
		else if (Window::GetKeyboard()->KeyDown(KEYBOARD_3)) {
			flag = 3;
		}


		renderer.UpdateScene(w.GetTimer()->GetTimedMS());
		renderer.RenderScene(flag);


	}

	return 0;
}