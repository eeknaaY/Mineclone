#include "Engine.h"

#include "Chunk.h"

int main() {
	Engine::Initialize();

	std::vector<SSBOMesh::Face> faces = std::vector<SSBOMesh::Face>{};

	enum Actions {
		RENDER_WIREFRAME,
		STOP_ENGINE
	};

	Engine::Keybind::SetKeybind(GLFW_KEY_G, Actions::RENDER_WIREFRAME);
	Engine::Keybind::SetAction(RENDER_WIREFRAME, []() { Mesh::ToggleWireframe(); });

	Engine::Keybind::SetKeybind(GLFW_KEY_ESCAPE, Actions::STOP_ENGINE);
	Engine::Keybind::SetAction(STOP_ENGINE, []() { Engine::Stop(); });

	Shader mainShader("../../Mineclone/src/Shaders/mainShader.vs", "../../Mineclone/src/Shaders/mainShader.fs");

	mainShader.use();
	mainShader.setMat4("ProjectionMatrix", Engine::MainCamera->getProjectionMatrix());

	Chunk test(glm::ivec3(0, 0, 0), 1);
	Chunk test2(glm::ivec3(0, 0, 1), 1);
	Chunk test3(glm::ivec3(1, 0, 0), 1);
	Chunk test4(glm::ivec3(0, 0, -1), 1);


	//for (int n = 0; n < 10000; n++) test.updateMesh();
	test.updateMesh();
	test.bindMesh();

	test2.updateMesh();
	test2.bindMesh();

	test3.updateMesh();
	test3.bindMesh();

	test4.updateMesh();
	test4.bindMesh();
	while (Engine::MainWindow->isActive()) {
		/* Main Loop Here */
		mainShader.setMat4("ViewMatrix", Engine::MainCamera->getViewMatrix());
		test.draw(mainShader);
		test2.draw(mainShader);
		test3.draw(mainShader);
		test4.draw(mainShader);


		// Scuffed reload shader for now, make it organized later.
		if (glfwGetKey(Engine::MainWindow->getRawWindow(), GLFW_KEY_R) == GLFW_PRESS) {
			mainShader = Shader("../../Mineclone/src/Shaders/mainShader.vs", "../../Mineclone/src/Shaders/mainShader.fs");
			mainShader.use();
			mainShader.setMat4("ProjectionMatrix", Engine::MainCamera->getProjectionMatrix());
			mainShader.setMat4("ViewMatrix", Engine::MainCamera->getViewMatrix());
		}

		Engine::Update();
	}

	return 0;
}