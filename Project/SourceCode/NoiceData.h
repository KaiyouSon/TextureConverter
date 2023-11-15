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

	// �u���b�N���U�C�N�p
	uint32_t blockSize;
};
