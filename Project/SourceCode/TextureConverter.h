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
	void SaveDDSTextureToFile(const uint32_t numOptions, char* options[]);

public:
	// �e�N�X�`����WIC����DDC�ɕϊ�����
	void ConvertTextureWICToDDC(const std::string& filrPath, const uint32_t numOptions = 0, char* options[] = nullptr);

public:
	// �g�p���@���o�́i�\���j����
	static void OutputUsage();

};

