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


