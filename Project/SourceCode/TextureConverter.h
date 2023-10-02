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
	void SaveDDSTextureToFile();

public:
	// テクスチャをWICからDDCに変換する
	void ConvertTextureWICToDDC(const std::string& filrPath);

};

