#pragma once
#include <DirectXTex.h>
#include "FilePathInfo.h"

struct TextureData
{
	// �摜�̏��
	DirectX::TexMetadata metadata;
	// �摜�̃C���[�W�̃R���e�i
	DirectX::ScratchImage scratchImage;

	WFilePathInfo wFilePathInfo;
};

