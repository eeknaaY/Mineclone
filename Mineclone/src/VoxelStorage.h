#pragma once

#include "Voxel.h"
#include "Engine.h"

#include <vector>
#include <bitset>
#include "glm/vec3.hpp"

class VoxelStorage {
public:
	// Stores the data as a cube.
	VoxelStorage(int storageSize);
	void setVoxel(const glm::ivec3& position, Voxel voxel);
	Voxel& getVoxel(const glm::ivec3& position);

	const static int BitsetValue = 64;
	std::bitset<BitsetValue> getVoxelRow(const glm::ivec2& position, Direction direction);
	
private:
	int m_StorageSize;
	std::vector<Voxel> m_Storage;
};