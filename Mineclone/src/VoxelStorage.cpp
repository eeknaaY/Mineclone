#include "VoxelStorage.h"

#include <cmath>

VoxelStorage::VoxelStorage(int storageSize, int dimensions, int defaultValue) {
	this->m_StorageSize = storageSize;
	this->m_DimensionCount = dimensions;

	// Initialize storage
	Voxel defaultVoxel = Voxel(defaultValue, VoxelType::TRANSPARENT);
	m_Storage = std::vector<Voxel>((int)pow(storageSize, dimensions), defaultVoxel);
}

void VoxelStorage::setVoxel(const glm::ivec3& position, const Voxel& voxel) {
	m_Storage[position.x + position.y * m_StorageSize + position.z * m_StorageSize * m_StorageSize] = voxel;
}

Voxel VoxelStorage::getVoxel(const glm::ivec3& position) const {
	static Voxel airVoxel = Voxel(0, VoxelType::TRANSPARENT);

	if (position.x < 0 || position.x > m_StorageSize) return airVoxel;
	if (position.y < 0 || position.y > m_StorageSize) return airVoxel;
	if (position.z < 0 || position.z > m_StorageSize) return airVoxel;


	return m_Storage[position.x + position.y * m_StorageSize + position.z * m_StorageSize * m_StorageSize];
}

uint64_t VoxelStorage::getVoxelRow(const glm::ivec2& position, Direction direction) {
	uint64_t returningValue = 0;
	switch (direction) {
	case Direction::NORTH:
		for (int z = 0; z < m_StorageSize; z++) {
			Voxel voxel = getVoxel(glm::vec3(position.x, position.y, z));
			if (voxel.blockID == 0) continue;

			returningValue |= (1ULL << z);
		}
		break;

	case Direction::WEST:
		for (int x = 0; x < m_StorageSize; x++) {
			Voxel voxel = getVoxel(glm::vec3(x, position.y, position.x));
			if (voxel.blockID == 0) continue;

			returningValue |= (1ULL << x);
		}
		break;
	case Direction::UP:
		for (int y = 0; y < m_StorageSize; y++) {
			Voxel voxel = getVoxel(glm::vec3(position.x, y, position.y));
			if (voxel.blockID == 0) continue;

			returningValue |= (1ULL << y);
		}
		break;
	}

	return returningValue;
}

void VoxelStorage::getVoxelLayer(const VoxelStorage& content, int depth, Direction direction) {
	if (m_StorageSize != content.m_StorageSize) {
#ifdef _DEBUG
		std::cout << "Attempting to get a voxel layer from two VoxelStorages that are different sizes.\n";
#endif // _DEBUG
		return;
	}

	switch (direction) {
	case Direction::NORTH:
		for (int y = 0; y < m_StorageSize; y++) {
			for (int x = 0; x < m_StorageSize; x++) {
				setVoxel(glm::vec3(x, y, depth), content.getVoxel(glm::ivec3(x, y, depth)));
			}
		}
		break;

	case Direction::WEST:
		for (int z = 0; z < m_StorageSize; z++) {
			for (int y = 0; y < m_StorageSize; y++) {
				setVoxel(glm::vec3(depth, y, z), content.getVoxel(glm::ivec3(depth, y, z)));
			}
		}
		break;
	case Direction::UP:
		for (int z = 0; z < m_StorageSize; z++) {
			for (int x = 0; x < m_StorageSize; x++) {
				setVoxel(glm::vec3(x, depth, z), content.getVoxel(glm::ivec3(x, depth, z)));
			}
		}
		break;
	}
}