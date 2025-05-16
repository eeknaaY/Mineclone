#pragma once

#include "Mesh.h"

class SSBOMesh : public Mesh {
public:
	struct Face {
		Face(const glm::vec3& position, const glm::vec2& size, Direction direction) {
			this->position = position;
			this->size = size;
			this->direction = direction;
		}

		// Center position of the face
		glm::vec3 position;
		glm::vec2 size;
		Direction direction;
	};

	SSBOMesh(const std::vector<Face>& faces);

	void draw() override;
	void bind() override;

private:
	unsigned int m_SSBO = 0;
};