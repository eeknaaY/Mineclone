#include "VoxelStorage.h"

VoxelStorage::VoxelStorage(int storageSize) {
	Voxel airVoxel = Voxel(0, VoxelType::TRANSPARENT);

	this->m_StorageSize = storageSize;

	// Initialize storage with air voxels
	m_Storage = std::vector<Voxel>(storageSize * storageSize * storageSize, airVoxel);
}

void VoxelStorage::setVoxel(const glm::ivec3& position, Voxel voxel) {
	m_Storage[position.x + position.y * m_StorageSize + position.z * m_StorageSize * m_StorageSize] = voxel;
}

Voxel& VoxelStorage::getVoxel(const glm::ivec3& position) {
	return m_Storage[position.x + position.y * m_StorageSize + position.z * m_StorageSize * m_StorageSize];
}

std::bitset<VoxelStorage::BitsetValue> VoxelStorage::getVoxelRow(const glm::ivec2& position, Direction direction) {
	std::bitset<BitsetValue> returningBitset;
	
	if (m_StorageSize > BitsetValue) {
#ifdef _DEBUG
		std::cout << "VoxelStorage size is too big for the current bitset size.\n";
#endif // _DEBUG
		return returningBitset;
	}

	switch (direction) {
	case Direction::NORTH:
		for (int z = 0; z < m_StorageSize; z++) {
			Voxel voxel = getVoxel(glm::vec3(position.x, position.y, z));
			if (voxel.blockID == 0) continue;

			returningBitset[z] = 1;
		}
		break;

	case Direction::WEST:
		for (int x = 0; x < m_StorageSize; x++) {
			Voxel voxel = getVoxel(glm::vec3(x, position.y, position.x));
			if (voxel.blockID == 0) continue;

			returningBitset[x] = 1;
		}
		break;
	case Direction::UP:
		for (int y = 0; y < m_StorageSize; y++) {
			Voxel voxel = getVoxel(glm::vec3(position.x, y, position.y));
			if (voxel.blockID == 0) continue;

			returningBitset[y] = 1;
		}
		break;
	}

	return returningBitset;
}