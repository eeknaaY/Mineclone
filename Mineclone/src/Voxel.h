#pragma once

#include "Engine.h"

enum class VoxelType {
	OPAQUE,
	TRANSPARENT
};

struct Voxel {
	Voxel(int blockID, VoxelType voxelType);
	Voxel(int blockID, Direction direction);

	void addDirection(int direction);
	void addDirection(Direction direction);

	void removeDirection(int direction);
	void removeDirection(Direction direction);

	bool hasDirection(int direction);
	bool hasDirection(Direction direction);

	int direction;
	VoxelType voxelType;
	int blockID;
};