#include "SSBOMesh.h"

SSBOMesh::SSBOMesh(std::vector<Face> faces) {
    for (const Face& face : faces) {

        glm::vec3 deltaPosition;
        switch (face.direction) {
            case Direction::NORTH:
                deltaPosition = glm::vec3(-face.size.x / 2, -face.size.y / 2, 0);
                break;
            case Direction::SOUTH:
                deltaPosition = glm::vec3(face.size.x / 2, -face.size.y / 2, 0);
                break;
            case Direction::EAST:
                deltaPosition = glm::vec3(0, -face.size.y / 2, -face.size.x / 2);
                break;
            case Direction::WEST:
                deltaPosition = glm::vec3(0, -face.size.y / 2, face.size.x / 2);
                break;
            case Direction::UP:
                deltaPosition = glm::vec3(-face.size.x / 2, 0, -face.size.y / 2);
                break;
            case Direction::DOWN:
                deltaPosition = glm::vec3(-face.size.x / 2, 0, face.size.y / 2);
                break;
        }


        Vertex vertex = Vertex(face.position + deltaPosition, face.size, face.direction);
        m_Vertices.push_back(vertex);
    }
}

void SSBOMesh::draw() {
    if (m_VAO == 0) this->bind();

    glBindVertexArray(m_VAO);

    // Needed?
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_SSBO);

    glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size() * 6);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
    glBindVertexArray(0);
}

void SSBOMesh::bind() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_SSBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_SSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}