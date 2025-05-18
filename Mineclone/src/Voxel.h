#pragma once

#include "Engine.h"

enum class VoxelType {
	OPAQUE,
	TRANSPARENT
};

struct Voxel {
	Voxel(int blockID, VoxelType voxelType);
	Voxel(int blockID, Direction direction);
	int direction;
	VoxelType voxelType;
	int blockID;
};