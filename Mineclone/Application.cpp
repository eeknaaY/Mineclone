#include "Engine.h"

#include <iostream>

int main() {
	Engine::Initialize();

	std::vector<SSBOMesh::Face> faces = std::vector<SSBOMesh::Face>{};

	enum Actions {
		STOP_ENGINE
	};

	Engine::Keybind::SetKeybind(GLFW_KEY_ESCAPE, Actions::STOP_ENGINE);
	Engine::Keybind::SetAction(STOP_ENGINE, []() { Engine::Stop(); });

	faces.push_back(SSBOMesh::Face(glm::vec3(0.25, 1, 0.5), glm::vec2(0.5, 2), Direction::NORTH));
	faces.push_back(SSBOMesh::Face(glm::vec3(0.25, 1, 0), glm::vec2(0.5, 2), Direction::SOUTH));
	faces.push_back(SSBOMesh::Face(glm::vec3(0, 1, 0.25), glm::vec2(0.5, 2), Direction::EAST));
	faces.push_back(SSBOMesh::Face(glm::vec3(0.5, 1, 0.25), glm::vec2(0.5, 2), Direction::WEST));
	faces.push_back(SSBOMesh::Face(glm::vec3(0.25, 2, 0.25), glm::vec2(0.5, 0.5), Direction::UP));
	faces.push_back(SSBOMesh::Face(glm::vec3(0.25, 0, 0.25), glm::vec2(0.5, 0.5), Direction::DOWN));

	SSBOMesh triangle = SSBOMesh(faces);

	// Shaders should probably be inside the mineclone project, not the engine
	// Could also make standard shaders, and you override them on the project
	Shader mainShader("src/Shaders/mainShader.vs", "src/Shaders/mainShader.fs");
	mainShader.use();
	mainShader.setMat4("ProjectionMatrix", Engine::MainCamera->getProjectionMatrix());

	while (Engine::MainWindow->isActive()) {
		/* Main Loop Here */
		mainShader.setMat4("ViewMatrix", Engine::MainCamera->getViewMatrix());
		mainShader.setMat4("TransformMatrix", glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)));
		triangle.draw();

		// Scuffed reload shader for now, make it organized later.
		if (glfwGetKey(Engine::MainWindow->getRawWindow(), GLFW_KEY_R) == GLFW_PRESS) {
			mainShader = Shader("D:/VisualStudio/Mineclone/Engine/src/Shaders/mainShader.vs", "D:/VisualStudio/Mineclone/Engine/src/Shaders/mainShader.fs");
			mainShader.use();
			mainShader.setMat4("ProjectionMatrix", Engine::MainCamera->getProjectionMatrix());
			mainShader.setMat4("ViewMatrix", Engine::MainCamera->getViewMatrix());
		}

		Engine::Update();
	}

	return 0;
}