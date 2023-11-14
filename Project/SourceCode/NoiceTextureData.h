#pragma once

enum class NoiceTextureType
{
	Mosaic,
};

struct NoiceTextureData
{
	NoiceTextureType type;
	float width, height;
};

