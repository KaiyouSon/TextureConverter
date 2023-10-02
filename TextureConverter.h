#pragma once
#include <string>

class TextureConverter
{
private:
	// テクスチャファイルの読み込み
	void LoadWICTextureFromFile(const std::string& filePath);

public:
	// テクスチャをWICからDDCに変換する
	void ConvertTextureWICToDDC(const std::string& filrPath);

};

