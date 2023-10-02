#include "TextureConverter.h"
#include "Util.h"
using namespace DirectX;

// テクスチャをWICからDDCに変換する
void TextureConverter::ConvertTextureWICToDDC(const std::string& filrPath)
{
	// テクスチャーロード
	LoadWICTextureFromFile(filrPath);

	// DDSテクスチャ出力
	SaveDDSTextureToFile();
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
void TextureConverter::SaveDDSTextureToFile()
{
	HRESULT result;

	// ミップマップ生成
	ScratchImage mipChain;
	result = GenerateMipMaps(
		mTextureData.scratchImage.GetImages(),
		mTextureData.scratchImage.GetImageCount(),
		mTextureData.scratchImage.GetMetadata(),
		TEX_FILTER_DEFAULT,
		0,
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


