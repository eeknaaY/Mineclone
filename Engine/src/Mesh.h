#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "glad/glad.h"

enum class Direction {
	NORTH, // +Z
	SOUTH, // -Z
	WEST,  // +X
	EAST,  // -X
	UP,    // +Y
	DOWN   // -Y
};

struct Vertex {
	Vertex(glm::vec3 position, glm::vec2 size, Direction faceDirection) {
		this->position = position;
		this->size = size;
		this->faceNormalIndex = (int)faceDirection;
	}

	// SSBO works with 16 bytes snippets, you need to add padding such that the GPU and CPU contains equal memory.

	// 12 + 4 Bytes
	glm::vec3 position;
	int faceNormalIndex;

	// 8 + 8 Bytes
	glm::vec2 size;
	glm::vec2 padding;
};

class Mesh {
public:
	virtual void draw() = 0;
	virtual void bind() = 0;

protected:
	std::vector<Vertex> m_Vertices;
	unsigned int m_VAO = 0;
};

