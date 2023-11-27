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

	uint32_t blockSize;		// �u���b�N�T�C�Y
	uint32_t fractalNum;	// �t���N�^����
};
