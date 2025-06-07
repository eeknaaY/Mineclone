#pragma once

#include "Engine.h"
#include "Voxel.h"
#include "VoxelStorage.h"

#include "glm/vec3.hpp"
#include <vector>

class ChunkManager;

class Chunk {
public:
	Chunk() = default;
	Chunk(const glm::ivec3& position);
	Chunk(const glm::ivec3& position, ChunkManager* chunkManager, int defaultVoxelValue);
	~Chunk();
	

	void setBlock(const glm::ivec3& position, const Voxel& voxel);
	Voxel getBlock(const glm::ivec3& position);
	glm::ivec3 getPosition() const;
	
	void draw(const Shader& shader);
	void updateMesh();
	void bindMesh();

private:
	static const int m_ChunkSize = 32;
	glm::ivec3 m_Position;

	// Iterate through X-Y-Z (Z furthest out)
	VoxelStorage m_VoxelData;
	ChunkManager* m_ChunkManager;

	SSBOMesh m_Mesh;
	// Meshing functions
	void createBlockFace(const SSBOMesh::Face& face);
	glm::ivec3 getDirectionPosition(const glm::ivec3& position, Direction direction);
};