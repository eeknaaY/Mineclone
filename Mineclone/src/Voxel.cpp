#include "Voxel.h"

Voxel::Voxel(int blockID, VoxelType voxelType) {
	this->voxelType = voxelType;
	this->blockID = blockID;
	this->direction = (int)Direction::NONE;
}

Voxel::Voxel(int blockID, Direction direction) {
	this->blockID = blockID;
	this->direction = (int)direction;
}