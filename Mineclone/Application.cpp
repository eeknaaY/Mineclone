#include "Engine.h"

#include "Chunk.h"
#include "ChunkManager.h"

int main() {
	Engine::Initialize();

	enum Actions {
		RENDER_WIREFRAME,
		STOP_ENGINE
	};

	Engine::ActionHandler::SetKeybind(GLFW_KEY_G, Actions::RENDER_WIREFRAME);
	Engine::ActionHandler::SetAction(RENDER_WIREFRAME, []() { Mesh::ToggleWireframe(); });

	Engine::ActionHandler::SetKeybind(GLFW_KEY_ESCAPE, Actions::STOP_ENGINE);
	Engine::ActionHandler::SetAction(STOP_ENGINE, []() { Engine::Stop(); });

	Shader mainShader("../../Mineclone/src/Shaders/mainShader.vs", "../../Mineclone/src/Shaders/mainShader.fs");

	mainShader.use();
	mainShader.setMat4("ProjectionMatrix", Engine::MainCamera->getProjectionMatrix());

	ChunkManager chunkStorage = ChunkManager();

	Chunk* newTest = chunkStorage.appendChunk(new Chunk(glm::ivec3(0, 0, 0), &chunkStorage, 1));
	Chunk* newTest2 = chunkStorage.appendChunk(new Chunk(glm::ivec3(1, 0, 0), &chunkStorage, 1));

	for (int z = 0; z < 32; z++) {
		for (int x = 0; x < 32; x++) {
			int maxHeight = rand() % 32;
			for (int y = 0; y < 32; y++) {
				if (y > maxHeight) newTest->setBlock(glm::ivec3(x, y, z), Voxel(0, VoxelType::OPAQUE));
			}
		}
	}

	newTest->updateMesh();
	newTest->bindMesh();

	newTest2->updateMesh();
	newTest2->bindMesh();

	while (Engine::MainWindow->isActive()) {
		/* Main Loop Here */
		mainShader.setMat4("ViewMatrix", Engine::MainCamera->getViewMatrix());
		chunkStorage.drawAllChunks(mainShader);


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