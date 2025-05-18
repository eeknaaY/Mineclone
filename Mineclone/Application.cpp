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

	Chunk test(glm::ivec3(0, 0, 0));
	for (int z = 0; z < 32; z++) {
		for (int y = 0; y < 32; y++) {
			for (int x = 0; x < 32; x++) {
				if ((x + y - z) % (x + 1) == 0) continue;
				test.setBlock(glm::ivec3(x, y, z), Voxel(1, VoxelType::OPAQUE));
			}
		}
	}
	test.updateMesh();

	while (Engine::MainWindow->isActive()) {
		/* Main Loop Here */
		mainShader.setMat4("ViewMatrix", Engine::MainCamera->getViewMatrix());
		mainShader.setMat4("TransformMatrix", glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)));
		test.draw(mainShader);

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