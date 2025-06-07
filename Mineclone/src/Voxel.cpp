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

void Voxel::addDirection(int direction) {
	this->direction |= direction;
}

void Voxel::removeDirection(int direction) {
	this->direction &= ~direction;
}

bool Voxel::hasDirection(int direction) {
	return (this->direction & direction) > 0;
}

void Voxel::addDirection(Direction direction) {
	addDirection((int)direction);
}

void Voxel::removeDirection(Direction direction) {
	removeDirection((int)direction);
}

bool Voxel::hasDirection(Direction direction) {
	return hasDirection((int)direction);
}