#include "ChunkManager.h"
#include "Chunk.h"

ChunkManager::ChunkManager() {

}

// ChunkManger takes ownership over the pointer.
Chunk* ChunkManager::appendChunk(Chunk* chunk) {
	m_ChunkStorage[chunk->getPosition()] = chunk;
	return chunk;
}

Chunk* ChunkManager::getChunk(const glm::ivec3& position) {
	try {
		return m_ChunkStorage.at(position);
	}
	catch (std::out_of_range oor) {
		return nullptr;
	}
}

void ChunkManager::removeChunk(const glm::ivec3& position) {
	if (Chunk* chunkToRemove = getChunk(position)) {
		delete chunkToRemove;
	}
}

void ChunkManager::drawAllChunks(const Shader& shader) {
	for (auto const& [key, val] : m_ChunkStorage){
		val->draw(shader);
	}
}