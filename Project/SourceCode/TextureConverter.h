#pragma once
#include <string>
#include "TextureData.h"

class TextureConverter
{
private:
	TextureData mTextureData;

private:
	// �e�N�X�`���t�@�C���̓ǂݍ���
	void LoadWICTextureFromFile(const std::string& filePath);

	// DDS�e�N�X�`���Ƃ��ăt�@�C���������o��
	void SaveDDSTextureToFile();

public:
	// �e�N�X�`����WIC����DDC�ɕϊ�����
	void ConvertTextureWICToDDC(const std::string& filrPath);

};

