#include "TextureConverter.h"

void TextureConverter::LoadWICTextureFromFile(const std::string& filePath)
{
	// ファイルパスをワイルド文字列に変改
	std::wstring wFilePath = std::wstring(filePath.begin(), filePath.end());

}

void TextureConverter::ConvertTextureWICToDDC(const std::string& filrPath)
{
	// テクスチャーロード
	LoadWICTextureFromFile(filrPath);

}
