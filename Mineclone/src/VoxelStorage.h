#pragma once

#include "Voxel.h"
#include "Engine.h"

#include <vector>
#include <bitset>
#include "glm/vec3.hpp"

class VoxelStorage {
public:
	// Stores the data as a cube.
	VoxelStorage() = default;
	VoxelStorage(int storageSize, int dimensions, int defaultVoxel = m_DefaultVoxelValue);
	void setVoxel(const glm::ivec3& position, const Voxel& voxel);
	Voxel getVoxel(const glm::ivec3& position) const;

	const static int BitsetValue = 64;
	uint64_t getVoxelRow(const glm::ivec2& position, Direction direction);

	void getVoxelLayer(const VoxelStorage& content, int depth, Direction direction);
	
private:
	static const int m_DefaultVoxelValue = 0;

	int m_DimensionCount;
	int m_StorageSize;
	std::vector<Voxel> m_Storage;
};