#pragma once

#include "Engine.h"
#include "Voxel.h"
#include "VoxelStorage.h"

#include "glm/vec3.hpp"
#include <vector>

class Chunk {
public:
	Chunk(const glm::ivec3& position);
	~Chunk();
	
	static const int m_ChunkSize = 32;

	void setBlock(const glm::ivec3& position, const Voxel& voxel);
	Voxel getBlock(const glm::ivec3& position);
	
	void draw(const Shader& shader);
	void updateMesh();

private:
	glm::ivec3 m_Position;

	// Iterate through X-Y-Z (Z furthest out)
	VoxelStorage m_VoxelData = VoxelStorage(m_ChunkSize);

	SSBOMesh m_Mesh;
	// Meshing functions
	void createBlockFace(const glm::ivec3& position, Direction direction);
	glm::ivec3 getDirectionPosition(const glm::ivec3& position, Direction direction);
};