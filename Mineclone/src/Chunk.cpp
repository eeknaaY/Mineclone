#include "Chunk.h"

#include "ChunkManager.h"

#include <stdexcept>
#include <chrono>


Chunk::Chunk(const glm::ivec3& position) {
	m_Position = position;
	m_VoxelData = VoxelStorage(m_ChunkSize, 3, 0);
}

Chunk::Chunk(const glm::ivec3& position, ChunkManager* chunkManager, int defaultVoxelValue) {
	m_Position = position;
	m_VoxelData = VoxelStorage(m_ChunkSize, 3, defaultVoxelValue);
	m_ChunkManager = chunkManager;
}

Chunk::~Chunk() {

}

void Chunk::setBlock(const glm::ivec3& position, const Voxel& voxel) {
	m_VoxelData.setVoxel(position, voxel);
}

Voxel Chunk::getBlock(const glm::ivec3& position) {
	return m_VoxelData.getVoxel(position);
}

glm::ivec3 Chunk::getPosition() const {
	return m_Position;
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
	for (int z = 0; z < m_ChunkSize; z++) {
		for (int y = 0; y < m_ChunkSize; y++) {
			for (int x = 0; x < m_ChunkSize; x++) {
				Voxel voxelData = getBlock(glm::vec3((x + m_ChunkSize) % m_ChunkSize, (y + m_ChunkSize) % m_ChunkSize, (z + m_ChunkSize) % m_ChunkSize));

				allVoxelData.setVoxel(glm::vec3(x + 1, y + 1, z + 1), voxelData);
			}
		}
	}

	for (unsigned int direction = (int)Direction::NORTH; direction <= (int)Direction::DOWN; direction <<= 1) {
		glm::ivec3 offset;
		switch ((Direction)direction) {
		case Direction::NORTH: offset = glm::ivec3(0, 0, 1); break;
		case Direction::SOUTH: offset = glm::ivec3(0, 0, -1); break;
		case Direction::UP:    offset = glm::ivec3(0, 1, 0); break;
		case Direction::DOWN:  offset = glm::ivec3(0, -1, 0); break;
		case Direction::WEST:  offset = glm::ivec3(1, 0, 0); break;
		case Direction::EAST:  offset = glm::ivec3(-1, 0, 0); break;
		}

		Chunk* neighborChunk = m_ChunkManager->getChunk(m_Position + offset);

		for (int y = 0; y < m_ChunkSize; y++) {
			for (int x = 0; x < m_ChunkSize; x++) {
				
				int chunkDepth = (direction & ((int)Direction::SOUTH | (int)Direction::DOWN | (int)Direction::EAST)) > 0 ? m_ChunkSize - 1 : 0;
				Voxel targetVoxel = Voxel(0, VoxelType::OPAQUE);
				if (neighborChunk) {
					glm::ivec3 getPosition = getDirectionPosition(glm::ivec3(x, y, chunkDepth), (Direction)direction);
					targetVoxel = neighborChunk->getBlock(getPosition);
				}	

				glm::ivec3 setPosition = getDirectionPosition(glm::ivec3(x + 1, y + 1, chunkDepth == 0 ? CSWP - 1 : 0), (Direction)direction);
				allVoxelData.setVoxel(setPosition, targetVoxel);
			}
		}
	}

	// Storage to keep track of where to build faces.
	VoxelStorage voxelFaces = VoxelStorage(m_ChunkSize, 3);

	// Doing every other direction since the bitdata can be reused for the opposite direction.
	for (unsigned int direction = (int)Direction::NORTH; direction <= (int)Direction::DOWN; direction <<= 2) {

		for (int y = 1; y < m_ChunkSize + 1; y++) {
			for (int x = 1; x < m_ChunkSize + 1; x++) {
				// Generate bit data
				uint64_t bitData = allVoxelData.getVoxelRow(glm::ivec2(x, y), (Direction)direction);
				uint64_t copiedBitData = bitData;

				bitData &= ~(bitData >> 1);
				copiedBitData &= ~(copiedBitData << 1);

				// Iterate through the bitdata
				for (int itrCount = 1; itrCount < m_ChunkSize + 1; itrCount++) {
					glm::ivec3 position = getDirectionPosition(glm::ivec3(x - 1, y - 1, itrCount - 1), (Direction)direction);

					if ((bitData >> itrCount) & 1) {
						Voxel currentStoredVoxel = voxelFaces.getVoxel(position);
						currentStoredVoxel.blockID = 1;
						currentStoredVoxel.addDirection(direction);
						voxelFaces.setVoxel(position, currentStoredVoxel);
					}
					
					if ((copiedBitData >> itrCount) & 1) {
						Voxel currentStoredVoxel = voxelFaces.getVoxel(position);
						currentStoredVoxel.blockID = 1;
						currentStoredVoxel.addDirection(direction << 1);
						voxelFaces.setVoxel(position, currentStoredVoxel);
					}
				}
			}
		}
	}

	for (unsigned int direction = (int)Direction::NORTH; direction <= (int)Direction::DOWN; direction <<= 1) {
		for (int depth = 0; depth < m_ChunkSize; depth++) {
			for (int y = 0; y < m_ChunkSize; y++) {
				for (int x = 0; x < m_ChunkSize; x++) {
					glm::vec3 newPosition = getDirectionPosition(glm::vec3(x, y, depth), (Direction)direction);
					Voxel currentVoxel = voxelFaces.getVoxel(newPosition);
					if (!currentVoxel.hasDirection(direction)) continue;

					currentVoxel.removeDirection(direction);
					voxelFaces.setVoxel(newPosition, currentVoxel);
					SSBOMesh::Face face = SSBOMesh::Face(newPosition, glm::vec2(1, 1), (Direction)direction);

					for (int dx = 1; x + dx < m_ChunkSize; dx++) {
						glm::vec3 nextPosition = getDirectionPosition(glm::vec3(x + dx, y, depth), (Direction)direction);
						Voxel nextVoxel = voxelFaces.getVoxel(nextPosition);

						if (!nextVoxel.hasDirection(direction)) break;

						nextVoxel.removeDirection(direction);
						voxelFaces.setVoxel(nextPosition, nextVoxel);
						face.size.x++;
					}

					for (int dy = 1; y + dy < m_ChunkSize; dy++) {
						for (int dx = 0; dx < face.size.x; dx++) {
							glm::vec3 nextPosition = getDirectionPosition(glm::vec3(x + dx, y + dy, depth), (Direction)direction);
							Voxel nextVoxel = voxelFaces.getVoxel(nextPosition);

							if (!nextVoxel.hasDirection(direction)) goto BREAK_GREEDY_MESHING;
						}

						for (int dx = 0; dx < face.size.x; dx++) {
							glm::vec3 nextPosition = getDirectionPosition(glm::vec3(x + dx, y + dy, depth), (Direction)direction);
							Voxel nextVoxel = voxelFaces.getVoxel(nextPosition);
							nextVoxel.removeDirection(direction);
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

#if 1 // Print meshing time.
	auto stopTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> ms_double = stopTime - startTime;
	printf("Meshing chunk (%i, %i, %i) took %.3f ms.\n", m_Position.x, m_Position.y, m_Position.z, ms_double.count());
#endif 

}