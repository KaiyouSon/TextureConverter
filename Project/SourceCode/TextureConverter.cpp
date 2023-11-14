#include "TextureConverter.h"
#include "Util.h"
#include "Random.h"
#include <iostream>
#include <filesystem>

using namespace DirectX;
using namespace std::filesystem;

// テクスチャをWICからDDCに変換する
void TextureConverter::ConvertTextureWICToDDC(
	const std::string& filePath,
	const std::string& outputPath)
{
	// テクスチャーロード
	mTextureDatas.emplace_back();

	// ロード
	LoadWICTextureFromFile(filePath);

	// 出力ファイル名を設定する
	std::string outputFilepath = mTextureDatas.back().filepathInfo.CompositeFilePath(outputPath, ".dds");

	// DDSテクスチャ出力
	SaveDDSTextureToFile(mTextureDatas.back().scratchImage, outputFilepath);

	mTextureDatas.clear();
}

// フォルダから3Dテクスチャを作成する
void TextureConverter::ConvertTo3DTexture(
	const std::vector<std::string>& filenames,
	const std::string& deirectroyPath,
	const std::string& outputPath)
{
	std::vector<std::string> localFilenames = filenames;

	// 名前順にソードする
	SortPNGFilenameToAscendingName(localFilenames);

	// ロード
	for (const auto& filename : localFilenames)
	{
		mTextureDatas.emplace_back();
		LoadWICTextureFromFile(deirectroyPath + filename);
	}

	ScratchImage scratchImage;

	// 統合
	CombineImagesTo3DTexture(scratchImage);

	// 圧縮
	CompressToBC7(scratchImage);

	// 出力ファイル名を設定する
	std::string outputFilepath = mTextureDatas.front().filepathInfo.CompositeFilePath(outputPath, ".dds");

	// 出力
	SaveDDSTextureToFile(scratchImage, outputFilepath);

	mTextureDatas.clear();
}

// ノイズテクスチャを生成する
void TextureConverter::CreateNoiceTexture(
	const NoiceTextureData& data,
	const std::string& filename,
	const std::string& outputPath)
{
	// 試しにモザイク生成

	DirectX::TexMetadata metadata;
	metadata.width = static_cast<uint32_t>(data.width);
	metadata.height = static_cast<uint32_t>(data.height);
	metadata.depth = 1;
	metadata.format = DXGI_FORMAT_R8_UNORM;
	metadata.mipLevels = 1;
	metadata.arraySize = 1;
	metadata.dimension = TEX_DIMENSION_TEXTURE2D;
	metadata.miscFlags = 0;
	metadata.miscFlags2 = 0;

	ScratchImage scratchImage;
	scratchImage.Initialize(metadata);

	uint8_t* imageData = scratchImage.GetPixels();

	uint32_t rowPitch = metadata.width * sizeof(uint8_t);
	for (uint32_t y = 0; y < metadata.height; y++)
	{
		for (uint32_t x = 0; x < metadata.width; x++)
		{
			imageData[x] = static_cast<uint8_t>(Random::Range(0, 255));
		}
		imageData += rowPitch;
	}

	std::string output = outputPath + filename + ".dds";
	SaveDDSTextureToFile(scratchImage, output);
}

// テクスチャファイルの読み込み
void TextureConverter::LoadWICTextureFromFile(const std::string& filePath)
{
	if (mTextureDatas.empty() == true)
	{
		return;
	}

	// ファイルパスをワイルド文字列に変改
	std::wstring wFilePath = std::wstring(filePath.begin(), filePath.end());

	// WICテクスチャロード
	HRESULT result = LoadFromWICFile(
		wFilePath.c_str(),
		WIC_FLAGS_NONE,
		&mTextureDatas.back().metadata,
		mTextureDatas.back().scratchImage);
	assert(SUCCEEDED(result));

	// フォルダパスとファイル名を分離する
	mTextureDatas.back().filepathInfo = SeparateWFilePath(wFilePath);
}

// DDSテクスチャとしてファイルを書き出す
void TextureConverter::SaveDDSTextureToFile(DirectX::ScratchImage& scratchImage, const std::string& outputPath)
{
	// 出力ファイル名を設定する
	std::wstring wOutputPath = std::wstring(outputPath.begin(), outputPath.end());

	// DDSファイル書き出し
	HRESULT result = SaveToDDSFile(
		scratchImage.GetImages(),
		scratchImage.GetImageCount(),
		scratchImage.GetMetadata(),
		DDS_FLAGS_NONE,
		wOutputPath.c_str());
	assert(SUCCEEDED(result));
}

// ScrateImageを結合する
void TextureConverter::CombineImagesTo3DTexture(ScratchImage& combinedImage)
{
	if (mTextureDatas.empty())
	{
		return;
	}

	DXGI_FORMAT format = mTextureDatas[0].scratchImage.GetMetadata().format;
	uint32_t width = mTextureDatas[0].scratchImage.GetMetadata().width;
	uint32_t height = mTextureDatas[0].scratchImage.GetMetadata().height;

	uint32_t totalDepth = mTextureDatas.size(); // 3Dテクスチャの深さ（スライス数）

	DirectX::TexMetadata metadata;
	metadata.width = static_cast<uint32_t>(width);
	metadata.height = static_cast<uint32_t>(height);
	metadata.depth = static_cast<uint32_t>(totalDepth);
	metadata.format = format;
	metadata.mipLevels = 1;
	metadata.arraySize = 1;
	metadata.dimension = TEX_DIMENSION_TEXTURE3D;
	metadata.miscFlags = 0;
	metadata.miscFlags2 = 0;

	// 統合する処理
	HRESULT hr = combinedImage.Initialize(metadata);
	for (size_t slice = 0; slice < totalDepth; ++slice)
	{
		const Image* srcImage = mTextureDatas[slice].scratchImage.GetImage(0, 0, 0);
		const Image* destImage = combinedImage.GetImage(0, 0, slice);
		uint32_t pixelSize = srcImage->rowPitch * srcImage->height;
		memcpy(destImage->pixels, srcImage->pixels, pixelSize);
	}
}

// --- 圧縮関連 ----------------------------------------------------------------------------------------- //
void TextureConverter::CompressToBC7(DirectX::ScratchImage& scratchImage, bool useMipMap)
{
	HRESULT result;

	// ミップマップ生成
	if (useMipMap == true)
	{
		ScratchImage mipChain;
		result = GenerateMipMaps(
			scratchImage.GetImages(),
			scratchImage.GetImageCount(),
			scratchImage.GetMetadata(),
			TEX_FILTER_DEFAULT,
			0,
			mipChain);

		if (SUCCEEDED(result))
		{
			// イメージとメタデータをミップマップ版で置き換える
			scratchImage = std::move(mipChain);	// コピー禁止なのでmoveする
		}
	}

	// 圧縮形式に変換
	TEX_COMPRESS_FLAGS flags =
		TEX_COMPRESS_BC7_QUICK |
		TEX_COMPRESS_SRGB_OUT |
		TEX_COMPRESS_PARALLEL;

	ScratchImage converted;
	result = Compress(
		scratchImage.GetImages(),
		scratchImage.GetImageCount(),
		scratchImage.GetMetadata(),
		DXGI_FORMAT_BC7_UNORM_SRGB,
		flags,
		1.0f,
		converted);

	auto meradata = scratchImage.GetMetadata();
	meradata.format = MakeSRGB(meradata.format);

	if (SUCCEEDED(result))
	{
		scratchImage = std::move(converted);	// コピー禁止なのでmoveする
	}
}

// 授業課題
void TextureConverter::ConvertTextureWICToDDC(const std::string& filePath, const uint32_t numOptions, char* options[])
{
	// テクスチャーロード
	LoadWICTextureFromFile(filePath);

	// DDSテクスチャ出力
	SaveDDSTextureToFile(numOptions, options);
}
void TextureConverter::SaveDDSTextureToFile(const uint32_t numOptions, char* options[])
{
	//HRESULT result;

	//// ミップマップレベル指定を検索
	//uint32_t mipLevel = 0;
	//for (uint32_t i = 0; i < numOptions; i++)
	//{
	//	if (std::string(options[i]) == "-ml")
	//	{
	//		// ミップレベル指定
	//		mipLevel = std::stoi(options[i + 1]);
	//		break;
	//	}
	//}

	//// ミップマップ生成
	//ScratchImage mipChain;
	//result = GenerateMipMaps(
	//	mTextureData.scratchImage.GetImages(),
	//	mTextureData.scratchImage.GetImageCount(),
	//	mTextureData.scratchImage.GetMetadata(),
	//	TEX_FILTER_DEFAULT,
	//	mipLevel,
	//	mipChain);

	//if (SUCCEEDED(result))
	//{
	//	// イメージとメタデータをミップマップ版で置き換える
	//	mTextureData.scratchImage = std::move(mipChain);	// コピー禁止なのでmoveする
	//	mTextureData.metadata = mTextureData.scratchImage.GetMetadata();
	//}

	//// 読み込んだテクスチャをSRGBとして扱う
	//mTextureData.metadata.format = MakeSRGB(mTextureData.metadata.format);

	//// 圧縮形式に変換
	//TEX_COMPRESS_FLAGS flags =
	//	TEX_COMPRESS_BC7_QUICK |
	//	TEX_COMPRESS_SRGB_OUT |
	//	TEX_COMPRESS_PARALLEL;

	//ScratchImage converted;
	//result = Compress(
	//	mTextureData.scratchImage.GetImages(),
	//	mTextureData.scratchImage.GetImageCount(),
	//	mTextureData.scratchImage.GetMetadata(),
	//	DXGI_FORMAT_BC7_UNORM_SRGB,
	//	flags,
	//	1.0f,
	//	converted);

	//if (SUCCEEDED(result))
	//{
	//	mTextureData.scratchImage = std::move(converted);	// コピー禁止なのでmoveする
	//	mTextureData.metadata = mTextureData.scratchImage.GetMetadata();
	//}

	// 出力ファイル名を設定する
	//std::wstring wFilePath = mTextureData.wFilePathInfo.CompositeFilePath(L".dds");

	//// DDSファイル書き出し
	//result = SaveToDDSFile(
	//	mTextureData.scratchImage.GetImages(),
	//	mTextureData.scratchImage.GetImageCount(),
	//	mTextureData.metadata,
	//	DDS_FLAGS_NONE,
	//	wFilePath.c_str());
	//assert(SUCCEEDED(result));
}
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