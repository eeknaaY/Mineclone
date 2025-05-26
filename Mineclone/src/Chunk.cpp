#include "Chunk.h"

#include <stdexcept>
#include <chrono>

Chunk::Chunk(const glm::ivec3& position) {
	m_Position = position;
	m_VoxelData = VoxelStorage(m_ChunkSize, 3);
}

Chunk::Chunk(const glm::ivec3& position, int defaultVoxelValue) {
	m_Position = position;
	m_VoxelData = VoxelStorage(m_ChunkSize, 3, defaultVoxelValue);
}

Chunk::~Chunk() {

}

void Chunk::setBlock(const glm::ivec3& position, const Voxel& voxel) {
	m_VoxelData.setVoxel(position, voxel);
}

Voxel Chunk::getBlock(const glm::ivec3& position) {
	return m_VoxelData.getVoxel(position);
}

glm::ivec3 Chunk::getDirectionPosition(const glm::ivec3& position, Direction direction) {
	// Position X and Y are position, Z is bit depth
	switch (direction) {
	case Direction::NORTH:
		return glm::vec3(position.x, position.y, position.z);
	case Direction::SOUTH:
		return glm::vec3(position.x, position.y, position.z);
	case Direction::WEST:
		return glm::vec3(position.z, position.y, position.x);
	case Direction::EAST:
		return glm::vec3(position.z, position.y, position.x);
	case Direction::UP:
		return glm::vec3(position.x, position.z, position.y);
	case Direction::DOWN:
		return glm::vec3(position.x, position.z, position.y);
	}

	return glm::vec3(0);
}

void Chunk::createBlockFace(const SSBOMesh::Face& face) {
	glm::vec3 averageFacePosition = face.position;

	switch (face.direction) {
	case Direction::NORTH:
		averageFacePosition += glm::vec3(face.size.x / 2, face.size.y / 2, 1);
		break;
	case Direction::SOUTH:
		averageFacePosition += glm::vec3(face.size.x / 2, face.size.y / 2, 0);
		break;
	case Direction::WEST:
		averageFacePosition += glm::vec3(1, face.size.y / 2, face.size.x / 2);
		break;
	case Direction::EAST:
		averageFacePosition += glm::vec3(0, face.size.y / 2, face.size.x / 2);
		break;
	case Direction::UP:
		averageFacePosition += glm::vec3(face.size.x / 2, 1, face.size.y / 2);
		break;
	case Direction::DOWN:
		averageFacePosition += glm::vec3(face.size.x / 2, 0, face.size.y / 2);
		break;
	}

	m_Mesh.append(SSBOMesh::Face(averageFacePosition, face.size, face.direction));
}

void Chunk::draw(const Shader& shader) {
	shader.setMat4("TransformMatrix", glm::translate(glm::mat4(1.0f), glm::vec3(m_ChunkSize) * (glm::vec3)m_Position));
	m_Mesh.draw();
}

void Chunk::bindMesh() {
	m_Mesh.bind();
}

void Chunk::updateMesh() {
	m_Mesh.clear();

	auto startTime = std::chrono::high_resolution_clock::now();

	// Chunk Size With Padding (+2)
	int CSWP = m_ChunkSize + 2;

	// Includes voxel data from neighboring chunks.
	// Remember curent chunk data begins at x, y, z = 1;
	VoxelStorage allVoxelData = VoxelStorage(CSWP, 3);

	// Set the data into the storage.
	// TODO: Set neighboring chunks data into it.
	for (int z = 0; z < m_ChunkSize; z++) {
		for (int y = 0; y < m_ChunkSize; y++) {
			for (int x = 0; x < m_ChunkSize; x++) {
				allVoxelData.setVoxel(glm::vec3(x + 1, y + 1, z + 1), m_VoxelData.getVoxel(glm::vec3(x, y, z)));
			}
		}
	}

	// Storage to keep track of where to build faces.
	VoxelStorage voxelFaces = VoxelStorage(CSWP, 3);

	for (unsigned int direction = (int)Direction::NORTH; direction <= (int)Direction::DOWN; direction <<= 2) {

		for (int y = 0; y < CSWP; y++) {
			for (int x = 0; x < CSWP; x++) {
				// Generate bit data
				uint64_t bitData = allVoxelData.getVoxelRow(glm::ivec2(x, y), (Direction)direction);
				uint64_t copiedBitData = bitData;

				bitData &= ~(bitData >> 1);
				copiedBitData &= ~(copiedBitData << 1);

				// Iterate through the bitdata
				for (int itrCount = 0; itrCount < CSWP; itrCount++) {
					glm::ivec3 position = getDirectionPosition(glm::ivec3(x, y, itrCount), (Direction)direction);

					if ((bitData >> itrCount) & 1) {
						Voxel currentStoredVoxel = voxelFaces.getVoxel(position);
						currentStoredVoxel.blockID = 1;
						currentStoredVoxel.direction |= direction;
						voxelFaces.setVoxel(position, currentStoredVoxel);
					}
					
					if ((copiedBitData >> itrCount) & 1) {
						Voxel currentStoredVoxel = voxelFaces.getVoxel(position);
						currentStoredVoxel.blockID = 1;
						currentStoredVoxel.direction |= (direction << 1);
						voxelFaces.setVoxel(position, currentStoredVoxel);
					}
				}
			}
		}
	}

	for (unsigned int direction = (int)Direction::NORTH; direction <= (int)Direction::DOWN; direction <<= 1) {
		for (int depth = 1; depth < m_ChunkSize + 1; depth++) {
			for (int y = 1; y < m_ChunkSize + 1; y++) {
				for (int x = 1; x < m_ChunkSize + 1; x++) {
					glm::vec3 newPosition = getDirectionPosition(glm::vec3(x, y, depth), (Direction)direction);
					Voxel currentVoxel = voxelFaces.getVoxel(newPosition);
					if (!(currentVoxel.direction & direction)) continue;

					currentVoxel.direction &= ~direction;
					voxelFaces.setVoxel(newPosition, currentVoxel);
					SSBOMesh::Face face = SSBOMesh::Face(newPosition, glm::vec2(1, 1), (Direction)direction);

					for (int dx = 1; dx < m_ChunkSize; dx++) {
						glm::vec3 nextPosition = getDirectionPosition(glm::vec3(x + dx, y, depth), (Direction)direction);
						Voxel nextVoxel = voxelFaces.getVoxel(nextPosition);

						if (!(nextVoxel.direction & direction)) break;

						nextVoxel.direction &= ~direction;
						voxelFaces.setVoxel(nextPosition, nextVoxel);
						face.size.x++;
					}

					for (int dy = 1; dy < m_ChunkSize; dy++) {
						for (int dx = 0; dx < face.size.x; dx++) {
							glm::vec3 nextPosition = getDirectionPosition(glm::vec3(x + dx, y + dy, depth), (Direction)direction);
							Voxel nextVoxel = voxelFaces.getVoxel(nextPosition);

							if (!(nextVoxel.direction & direction)) goto BREAK_GREEDY_MESHING;
						}

						for (int dx = 0; dx < face.size.x; dx++) {
							glm::vec3 nextPosition = getDirectionPosition(glm::vec3(x + dx, y + dy, depth), (Direction)direction);
							Voxel nextVoxel = voxelFaces.getVoxel(nextPosition);
							nextVoxel.direction &= ~direction;
							voxelFaces.setVoxel(nextPosition, nextVoxel);
						}

						face.size.y++;
					}

					BREAK_GREEDY_MESHING:
					createBlockFace(face);
				}
			}
		}
	}

	// Avg meshing time - 2-5ms

#if _DEBUG // Print meshing time.
	auto stopTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> ms_double = stopTime - startTime;
	printf("Meshing chunk (%i, %i, %i) took %.3f ms.\n", m_Position.x, m_Position.y, m_Position.z, ms_double.count());
#endif 

}