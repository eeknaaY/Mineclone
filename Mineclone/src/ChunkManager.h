#pragma once

#include <unordered_map>
#include "glm/glm.hpp"
#include "Shader.h"

#include <functional>

namespace std {
	template<>
	struct hash<glm::ivec3> {
		std::size_t operator()(const glm::ivec3& v) const noexcept {
			std::size_t h1 = std::hash<int>{}(v.x);
			std::size_t h2 = std::hash<int>{}(v.y);
			std::size_t h3 = std::hash<int>{}(v.z);

			return h1 ^ (h2 << 1) ^ (h3 << 2);
		}
	};
}

class Chunk;

class ChunkManager {
public:
	ChunkManager();
	Chunk* appendChunk(Chunk* chunk);
	Chunk* getChunk(const glm::ivec3& position);
	void removeChunk(const glm::ivec3& position);
	void drawAllChunks(const Shader& shader);

private:
	std::unordered_map<glm::ivec3, Chunk*> m_ChunkStorage;
};