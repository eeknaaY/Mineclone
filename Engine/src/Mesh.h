#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "glad/glad.h"
#include <bit>

enum class Direction {
	NONE = 0,
	NORTH = 1 << 0, // +Z
	SOUTH = 1 << 1, // -Z
	WEST = 1 << 2,  // +X
	EAST = 1 << 3,  // -X
	UP = 1 << 4,    // +Y
	DOWN = 1 << 5   // -Y
};

struct Vertex {
	Vertex(const glm::vec3& position, const glm::vec2& size, Direction faceDirection) {
		this->position = position;
		this->size = size;
		this->faceNormalIndex = std::countr_zero((unsigned int)faceDirection);
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

	static void ToggleWireframe() {
		if (m_RenderingWireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		m_RenderingWireframe = !m_RenderingWireframe;
	}

protected:
	std::vector<Vertex> m_Vertices;
	unsigned int m_VAO = 0;

private:
	static bool m_RenderingWireframe;
};

