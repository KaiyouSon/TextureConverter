#pragma once
#include <DirectXTex.h>
#include "FilePathInfo.h"

struct TextureData
{
	// 画像の情報
	DirectX::TexMetadata metadata;
	// 画像のイメージのコンテナ
	DirectX::ScratchImage scratchImage;

	WFilePathInfo wFilePathInfo;
};

