#include "TextureConverter.h"
#include "Util.h"
#include <iostream>
using namespace DirectX;

// テクスチャをWICからDDCに変換する
void TextureConverter::ConvertTextureWICToDDC(const std::string& filrPath, const uint32_t numOptions, char* options[])
{
	// テクスチャーロード
	LoadWICTextureFromFile(filrPath);

	// DDSテクスチャ出力
	SaveDDSTextureToFile(numOptions, options);
}

// 使用方法を出力（表示）する
void TextureConverter::OutputUsage()
{
	std::cout << "画像ファイルをWIC形式からDDS形式に変換します。" << std::endl;
	std::cout << std::endl;

	std::cout << "TextureConverter [ドライブ:][パス][ファイル名] [-ml level]" << std::endl;
	std::cout << std::endl;

	std::cout << "[ドライブ:][パス][ファイル名]: を変換したいWIC形式の画像ファイルを指定します" << std::endl;
	std::cout << std::endl;

	std::cout << "[-ml level]:ミップレベルを指定します。0を指定すると1x1までのフルミップマップチェーンを生成します" << std::endl;
	std::cout << std::endl;
}

// テクスチャファイルの読み込み
void TextureConverter::LoadWICTextureFromFile(const std::string& filePath)
{
	HRESULT result;

	// ファイルパスをワイルド文字列に変改
	std::wstring wFilePath = std::wstring(filePath.begin(), filePath.end());

	// WICテクスチャロード
	result = LoadFromWICFile(wFilePath.c_str(), WIC_FLAGS_NONE, &mTextureData.metadata, mTextureData.scratchImage);
	assert(SUCCEEDED(result));

	// フォルダパスとファイル名を分離する
	mTextureData.wFilePathInfo = SeparateWFilePath(wFilePath);
}

// DDSテクスチャとしてファイルを書き出す
void TextureConverter::SaveDDSTextureToFile(const uint32_t numOptions, char* options[])
{
	HRESULT result;

	// ミップマップレベル指定を検索
	uint32_t mipLevel = 0;
	for (uint32_t i = 0; i < numOptions; i++)
	{
		if (std::string(options[i]) == "-ml")
		{
			// ミップレベル指定
			mipLevel = std::stoi(options[i + 1]);
			break;
		}
	}

	// ミップマップ生成
	ScratchImage mipChain;
	result = GenerateMipMaps(
		mTextureData.scratchImage.GetImages(),
		mTextureData.scratchImage.GetImageCount(),
		mTextureData.scratchImage.GetMetadata(),
		TEX_FILTER_DEFAULT,
		mipLevel,
		mipChain);

	if (SUCCEEDED(result))
	{
		// イメージとメタデータをミップマップ版で置き換える
		mTextureData.scratchImage = std::move(mipChain);	// コピー禁止なのでmoveする
		mTextureData.metadata = mTextureData.scratchImage.GetMetadata();
	}

	// 圧縮形式に変換
	TEX_COMPRESS_FLAGS flags =
		TEX_COMPRESS_BC7_QUICK |
		TEX_COMPRESS_SRGB_OUT |
		TEX_COMPRESS_PARALLEL;

	ScratchImage converted;
	result = Compress(
		mTextureData.scratchImage.GetImages(),
		mTextureData.scratchImage.GetImageCount(),
		mTextureData.scratchImage.GetMetadata(),
		DXGI_FORMAT_BC7_UNORM_SRGB,
		flags,
		1.0f,
		converted);

	if (SUCCEEDED(result))
	{
		mTextureData.scratchImage = std::move(converted);	// コピー禁止なのでmoveする
		mTextureData.metadata = mTextureData.scratchImage.GetMetadata();
	}

	// 読み込んだテクスチャをSRGBとして扱う
	mTextureData.metadata.format = MakeSRGB(mTextureData.metadata.format);

	// 出力ファイル名を設定する
	std::wstring wFilePath = mTextureData.wFilePathInfo.CompositeFilePath(L".dds");

	// DDSファイル書き出し
	result = SaveToDDSFile(
		mTextureData.scratchImage.GetImages(),
		mTextureData.scratchImage.GetImageCount(),
		mTextureData.metadata,
		DDS_FLAGS_NONE,
		wFilePath.c_str());
	assert(SUCCEEDED(result));
}


