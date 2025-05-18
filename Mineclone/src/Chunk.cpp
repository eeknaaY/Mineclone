#include "Chunk.h"

#include <stdexcept>
#include <chrono>

Chunk::Chunk(const glm::ivec3& position) {
	m_Position = position;
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
	case Direction::WEST:
		return glm::vec3(position.z, position.y, position.x);
	case Direction::UP:
		return glm::vec3(position.x, position.z, position.y);
	}

	return glm::vec3(0);
}

void Chunk::createBlockFace(const glm::ivec3& position, Direction direction) {
	glm::vec3 averageFacePosition = position;

	switch (direction) {
	case Direction::NORTH:
		averageFacePosition += glm::vec3(0.5, 0.5, 1);
		break;
	case Direction::SOUTH:
		averageFacePosition += glm::vec3(0.5, 0.5, 0);
		break;
	case Direction::WEST:
		averageFacePosition += glm::vec3(1, 0.5, 0.5);
		break;
	case Direction::EAST:
		averageFacePosition += glm::vec3(0, 0.5, 0.5);
		break;
	case Direction::UP:
		averageFacePosition += glm::vec3(0.5, 1, 0.5);
		break;
	case Direction::DOWN:
		averageFacePosition += glm::vec3(0.5, 0, 0.5);
		break;
	}

	m_Mesh.append(SSBOMesh::Face(averageFacePosition, glm::vec2(1, 1), direction));
}

void Chunk::draw(const Shader& shader) {
	shader.setMat4("TransformMatrix", glm::translate(glm::mat4(1.0f), (glm::vec3)m_Position));
	m_Mesh.draw();
}



void Chunk::updateMesh() {
	auto startTime = std::chrono::high_resolution_clock::now();

	// Chunk Size With Padding (+2)
	int CSWP = m_ChunkSize + 2;

	// Includes voxel data from neighboring chunks.
	// Remember curent chunk data begins at x, y, z = 1;
	VoxelStorage allVoxelData = VoxelStorage(CSWP);

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
	VoxelStorage voxelFaces = VoxelStorage(CSWP);

	for (int direction = (int)Direction::NORTH; direction <= (int)Direction::DOWN; direction <<= 2) {

		for (int y = 0; y < CSWP; y++) {
			for (int x = 0; x < CSWP; x++) {
				// Generate bit data
				std::bitset<VoxelStorage::BitsetValue> Bitdata = allVoxelData.getVoxelRow(glm::ivec2(x, y), (Direction)direction);
				std::bitset<VoxelStorage::BitsetValue> CopiedBitdata = Bitdata;

				Bitdata &= ~(Bitdata >> 1);
				CopiedBitdata &= ~(CopiedBitdata << 1);

				// Iterate through the bitdata
				for (int itrCount = 0; itrCount < CSWP; itrCount++) {
					glm::ivec3 position = getDirectionPosition(glm::ivec3(x, y, itrCount), (Direction)direction);

					if (Bitdata[itrCount] != 0) {
						// TODO: Change glm::ivec3(x, y, itrCount) so that itrCount is in correct position (z for X-Axis, x for Z-Axis, y for Y-Axis)
						Voxel currentStoredVoxel = voxelFaces.getVoxel(position);
						currentStoredVoxel.blockID = 1;
						currentStoredVoxel.direction |= direction;
						voxelFaces.setVoxel(position, currentStoredVoxel);
					}
					
					if (CopiedBitdata[itrCount] != 0) {
						Voxel currentStoredVoxel = voxelFaces.getVoxel(position);
						currentStoredVoxel.blockID = 1;
						currentStoredVoxel.direction |= (direction << 1);
						voxelFaces.setVoxel(position, currentStoredVoxel);
					}
				}
			}
		}
	}

	for (int z = 0; z < m_ChunkSize; z++) {
		for (int y = 0; y < m_ChunkSize; y++) {
			for (int x = 0; x < m_ChunkSize; x++) {
				Voxel storedVoxel = voxelFaces.getVoxel(glm::ivec3(x + 1, y + 1, z + 1));
				if (storedVoxel.blockID == 0) continue;

				std::bitset<6> voxelDirectionBitdata = std::bitset<6>(storedVoxel.direction);

				for (int directionIndex = 0; directionIndex < 6; directionIndex++) {
					// Work with the data we got from binary meshing
					// TODO: Implement greedy meshing
					if (voxelDirectionBitdata[directionIndex] == 0) continue;
					createBlockFace(glm::ivec3(x, y, z), (Direction)(1 << directionIndex));
				}
			}
		}
	}

	m_Mesh.bind();

	// Binary Meshing w/ Greedy Meshing - 4ms

#if 1 // Print meshing time.
	auto stopTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> ms_double = stopTime - startTime;
	printf("Meshing chunk (%i, %i, %i) took %.3f ms.\n", m_Position.x, m_Position.y, m_Position.z, ms_double.count());
#endif 

}