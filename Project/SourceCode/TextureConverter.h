#pragma once
#include <string>
#include "TextureData.h"

class TextureConverter
{
private:
	TextureData mTextureData;

private:
	// テクスチャファイルの読み込み
	void LoadWICTextureFromFile(const std::string& filePath);

	// DDSテクスチャとしてファイルを書き出す
	void SaveDDSTextureToFile(const uint32_t numOptions, char* options[]);
	void SaveDDSTextureToFile(const std::string& outputPath);

public:
	// テクスチャをWICからDDCに変換する
	void ConvertTextureWICToDDC(const std::string& filrPath, const uint32_t numOptions = 0, char* options[] = nullptr);
	void ConvertTextureWICToDDC(const std::string& filrPath, const std::string& outputPath);

public:
	// 使用方法を出力（表示）する
	static void OutputUsage();

};

