#pragma once
#include <stdint.h>

enum class NoiceTextureType
{
	Min,
	Mosaic,
	Block,
	Value,
	Perlin,
	Max,
};

struct NoiceData
{
	NoiceTextureType type;
	float width, height;

	// ブロックモザイク用
	uint32_t blockSize;
};
