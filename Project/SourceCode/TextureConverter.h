#pragma once
#include <string>
#include <vector>
#include "TextureData.h"
#include "NoiceData.h"

class TextureConverter
{
private:
	std::vector<TextureData> mTextureDatas;

private:
	// テクスチャファイルの読み込み
	void LoadWICTextureFromFile(const std::string& filePath);

	// DDSテクスチャとしてファイルを書き出す
	void SaveDDSTextureToFile(DirectX::ScratchImage& scratchImage, const std::string& outputPath);

private: // 圧縮関連
	void CompressToBC4(DirectX::ScratchImage& scratchImage);
	void CompressToBC7(DirectX::ScratchImage& scratchImage, bool useMipMap = true);

private: // その他

	// ScrateImageを結合する
	void CombineImagesTo3DTexture(DirectX::ScratchImage& combinedImage);

private: // ノイズテクスチャ関連

	// モザイクノイズ
	void CreateMosaicNoice(DirectX::ScratchImage& scratchImage, const NoiceData& data);

	// ブロックノイズ
	void CreateBlockNoice(DirectX::ScratchImage& scratchImage, const NoiceData& data);

	// バリューノイズ
	void CreateValueNoice(DirectX::ScratchImage& scratchImage, const NoiceData& data);

	// パーリンノイズ
	void CreatePerlinNoice(DirectX::ScratchImage& scratchImage, const NoiceData& data);

public: // 機能

	// テクスチャをWICからDDCに変換する
	void ConvertTextureWICToDDC(
		const std::string& filePath,
		const std::string& outputPath);

	// 複数枚のテクスチャを3Dテクスチャにする
	void ConvertTo3DTexture(
		const std::vector<std::string>& filenames,
		const std::string& deirectroyPath,
		const std::string& outputPath);

	// ハイトテクスチャを作成する
	void CreateHeightTexture(
		const std::string& filePath,
		const std::string& outputPath);

	// ノイズテクスチャを生成する
	void CreateNoiceTexture(
		const NoiceData& data,
		const std::string& filename,
		const std::string& outputPath);

public:
	// 授業コード
	void ConvertTextureWICToDDC(const std::string& filePath, const uint32_t numOptions = 0, char* options[] = nullptr);
	void SaveDDSTextureToFile(const uint32_t numOptions, char* options[]);

public:
	// 使用方法を出力（表示）する
	static void OutputUsage();

};

