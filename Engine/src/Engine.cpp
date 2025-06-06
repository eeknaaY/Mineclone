#include "Engine.h"

namespace Engine {

	std::shared_ptr<Camera> MainCamera = nullptr;
	std::shared_ptr<GraphicsWindow> MainWindow = nullptr;

	GraphicsWindow& Initialize() {

		MainWindow = std::shared_ptr<GraphicsWindow>(new GraphicsWindow());
		MainCamera = std::shared_ptr<Camera>(new Camera(MainWindow));


		return *MainWindow;
	}

	void Stop() {
		MainWindow->destroy();
		//MainWindow.reset();
		//MainCamera.reset();
	}

	void Update() {
		MainCamera->update();
		MainWindow->swap();
		MainWindow->pollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

}







