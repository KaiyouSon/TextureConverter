#include "TextureConverter.h"

void TextureConverter::LoadWICTextureFromFile(const std::string& filePath)
{
	// �t�@�C���p�X�����C���h������ɕω�
	std::wstring wFilePath = std::wstring(filePath.begin(), filePath.end());

}

void TextureConverter::ConvertTextureWICToDDC(const std::string& filrPath)
{
	// �e�N�X�`���[���[�h
	LoadWICTextureFromFile(filrPath);

}
