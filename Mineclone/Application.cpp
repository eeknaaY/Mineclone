#include "Engine.h"

#include <iostream>

int main() {
	Engine::Initialize();

	std::vector<SSBOMesh::Face> faces = std::vector<SSBOMesh::Face>{};

	faces.push_back(SSBOMesh::Face(glm::vec3(-0.5, 0.5, 1), glm::vec2(1, 1), Direction::NORTH));
	faces.push_back(SSBOMesh::Face(glm::vec3(-0.5, 0.5, 0), glm::vec2(1, 1), Direction::SOUTH));
	faces.push_back(SSBOMesh::Face(glm::vec3(-1, 0.5, 0.5), glm::vec2(1, 1), Direction::EAST));
	faces.push_back(SSBOMesh::Face(glm::vec3(0, 0.5, 0.5), glm::vec2(1, 1), Direction::WEST));
	faces.push_back(SSBOMesh::Face(glm::vec3(0.5, 1, 0.5), glm::vec2(1, 1), Direction::UP));
	faces.push_back(SSBOMesh::Face(glm::vec3(0.5, 0, 0.5), glm::vec2(1, 1), Direction::DOWN));

	SSBOMesh triangle = SSBOMesh(faces);


	Shader mainShader("D:/VisualStudio/Mineclone/Engine/src/Shaders/mainShader.vs", "D:/VisualStudio/Mineclone/Engine/src/Shaders/mainShader.fs");
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