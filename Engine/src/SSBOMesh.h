#pragma once

#include "Mesh.h"

class SSBOMesh : public Mesh {
public:
	struct Face {
		Face(glm::vec3 position, glm::vec2 size, Direction direction) {
			this->position = position;
			this->size = size;
			this->direction = direction;
		}

		glm::vec3 position;
		glm::vec2 size;
		Direction direction;
	};

	SSBOMesh(std::vector<Face> faces);

	void draw() override;
	void bind() override;

private:
	unsigned int m_SSBO = 0;
};